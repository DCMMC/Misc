package tk.dcmmc;

import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.concurrent.*;

/**
 * 读者
 */
class Reader implements Callable {
    private Integer id;
    private Double duration;
    private Semaphore rMutex;
    private Semaphore wMutex;
    private Container<Integer> readCounter;

    /**
     * 读者
     *
     * @param id 线程序号
     * @param duration 持续时间
     * @param rMutex 读的时候修改 readCounter 的时候用的互斥锁
     * @param wMutex 写互斥锁
     * @param readCounter 读线程计数器
     */
    Reader(Integer id, Double duration, Semaphore rMutex, Semaphore wMutex, Container<Integer> readCounter) {
        this.id = id;
        this.duration = duration;
        this.rMutex = rMutex;
        this.wMutex = wMutex;
        this.readCounter = readCounter;
    }

    @Override
    public Object call() {
        try {
            // P 操作, 为 readCounter 创造一个临界区
            rMutex.acquire();
            if (readCounter.item == 0) {
                // 如果现在只有一个 reader 进来读了, 像判断一下是否有 writer 在写
                // 否则堵塞
                wMutex.acquire();
            }
            // 下面这句话必须在 wMutex.acquire() 之后, 因为在写的时候要堵塞所有读线程
            readCounter.item++;
            // V 操作
            rMutex.release();

            // output information(simulate reading)
            System.out.println("Thread " + id + " 正在读取, 读取时间为 " + duration + "s, 当前正在读取的线程有 "
                    + readCounter.item + "个, rMutex: " + rMutex.availablePermits() + ", wMutex: "
                    + wMutex.availablePermits() + ", 当前时间: "
                    + new SimpleDateFormat("HH:mm:ss:SSS").format(new Date()));
            Thread.sleep((long)(duration * 1000));

            rMutex.acquire();
            readCounter.item--;
            rMutex.release();
            if (readCounter.item == 0) {
                // 如果没有读线程了, 就signal wMutex
                wMutex.release();
            }
        } catch (InterruptedException ie) {
            ie.printStackTrace();
        }
        return "Reader!";
    }
}

/**
 * 写者
 */
class Writer implements Callable {
    private Integer id;
    private Double duration;
    private Semaphore wMutex;
    private Semaphore rMutex;

    /**
     * 写者
     *
     * @param id 线程序号
     * @param duration 持续时间
     * @param wMutex 写互斥锁
     */
    Writer(Integer id, Double duration, Semaphore rMutex, Semaphore wMutex) {
        this.id = id;
        this.duration = duration;
        this.wMutex = wMutex;
        this.rMutex = rMutex;
    }

    @Override
    public Object call() {
        try {
            wMutex.acquire();
            // output information(simulate writer)
            System.out.println("Thread " + id + " 正在写入, 写入时间为 " + duration + "s"
                    + ", rMutex: " + rMutex.availablePermits() + ", wMutex: "
                    + wMutex.availablePermits() + ", 当前时间: "
                    + new SimpleDateFormat("HH:mm:ss:SSS").format(new Date()));
            Thread.sleep((long)(duration * 1000));
            wMutex.release();
        } catch (InterruptedException ie) {
            ie.printStackTrace();
        }
        return "Writer!";
    }
}

/**
 * Lab2
 * 用信号量机制来解决 Reader-Writer 问题
 * 限制:
 * 写-写互斥
 * 读-写互斥
 * 读-读允许
 * 如果一个读者申请进行读操作时已有另一个读者正在进行读操作，则该读者可直接开始读操作
 * 如果一个读者申请进行读操作时已有另一写者在等待访问共享资源，则该读者必须等到没有写者处于等待状态后才能开始读操作
 *
 * 运行结果显示要求:
 * 要求在每个线程创建、发出读写操作申请、开始读写操作和结束读写操作时分别显示一行提示信息，以确定所有处理都遵守相应的读写操作限制
 */
public class Lab2 {
    /**
     * start entry
     * @param args nullable
     */
    public static void main(String[] args) {
        // read from file
        String fileName = "Lab2_Test.txt";
        URL path = Lab2.class.getClassLoader().getResource(fileName);
        if (path != null) {
            try (Scanner sc = new Scanner(Paths.get(path.toURI()))) {
                // TimerTask 为 Reader 或 Writer, Double 为持续时间
                LinkedList<Map.Entry<Callable<?>, Double>> taskQueue = new LinkedList<>();
                // 信号量
                final Semaphore rMutex = new Semaphore(1), wMutex = new Semaphore(1);
                Container<Integer> readCounter = new Container<>(0);

                int id;
                char type;
                double duration;
                double start;
                while (sc.hasNext()) {
                    // 数据一定要合法
                    // 每一行的格式: 线程序号 R/W(表示 Reader 或 Writer 不区分大小写) 线程开始时间 线程持续时间
                    id = sc.nextInt();
                    type = sc.next().charAt(0);
                    start = sc.nextDouble();
                    duration = sc.nextDouble();
                    if (type == 'R' || type == 'r')
                        taskQueue.addLast(new AbstractMap.SimpleEntry<Callable<?>, Double>(new Reader(id, duration,
                                rMutex, wMutex, readCounter), start));
                    else if (type == 'W' || type == 'w')
                        taskQueue.addLast(new AbstractMap.SimpleEntry<Callable<?>, Double>(new Writer(id, duration,
                                rMutex, wMutex), start));
                }
                ScheduledExecutorService scheduledExecutorPool = Executors.newScheduledThreadPool(taskQueue.size());
                for (Map.Entry<Callable<?>, Double> i : taskQueue) {
                    scheduledExecutorPool.schedule(i.getKey(), (long)(i.getValue() * 1000), TimeUnit.MILLISECONDS);
                }
                // mother fucker
                // 一定要记得 shutdown, 不然这傻逼线程池不会停...
                scheduledExecutorPool.shutdown();
            } catch (URISyntaxException | IOException e) {
                e.printStackTrace();
            }
        }
    }
}///~
