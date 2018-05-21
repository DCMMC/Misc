package tk.dcmmc;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Semaphore;
import java.util.concurrent.ThreadLocalRandom;

/**
 * 生产者
 * @param <Item> 共享缓冲区的元素类型, i.e. 生产者生产的元素的类型
 */
class Producer<Item> implements Runnable {
    private Container<Item[]> buffer;
    private Semaphore mutex, empty, full;
    // 生产者的索引
    private Container<Integer> in;
    // 编号
    private int num;

    Producer(Container<Item[]> buffer, Semaphore mutex, Semaphore empty, Semaphore full, Container<Integer> in,
             int num) {
        this.buffer = buffer;
        this.mutex = mutex;
        this.empty = empty;
        this.full = full;
        this.in = in;
        this.num = num;
    }

    @SuppressWarnings("unchecked")
    @Override
    public void run() {
        int threshold = 15, count = 1;
        while (true) {
            // 生成一个数据
            Item newData = null;
            if (buffer.item.length > 0 && buffer.item[0] instanceof Integer)
                newData = (Item) (Integer.valueOf(ThreadLocalRandom.current().nextInt(1,
                        buffer.item.length + 1)));
            try {
                // P 操作
                empty.acquire();
                mutex.acquire();

                // log
                System.out.println("生产者 " + num + " 生成了一个数据: buffer[" + in.item + "] = " + newData);
                System.out.println("----------------------------------------------------------");

                buffer.item[in.item] = newData;
                in.item = (in.item + 1) % buffer.item.length;

                // 休眠一下子
                Thread.sleep(150);

                // V 操作
                mutex.release();
                full.release();

                if (count++ >= threshold)
                    break;
            } catch (InterruptedException ie) {
                ie.printStackTrace();
            }
        }

    }
}

/**
 * 消费者
 * @param <Item> 共享缓冲区的元素类型, i.e. 生产者生产的元素的类型
 */
class Consumer<Item> implements Runnable {
    private Container<Item[]> buffer;
    private Semaphore mutex, empty, full;
    // 消费者的索引
    private Container<Integer> out;
    // 编号
    private int num;

    Consumer(Container<Item[]> buffer, Semaphore mutex, Semaphore empty, Semaphore full, Container<Integer> out,
             int num) {
        this.buffer = buffer;
        this.mutex = mutex;
        this.empty = empty;
        this.full = full;
        this.out = out;
        this.num = num;
    }

    @SuppressWarnings("unchecked")
    @Override
    public void run() {
        int threshold = 10, count = 1;

        while (true) {
            try {
                // P 操作
                full.acquire();
                mutex.acquire();
                // 取出元素
                Item data = buffer.item[out.item];
                buffer.item[out.item] = (Item) Integer.valueOf(0);
                out.item = (out.item + 1) % buffer.item.length;
                Item[] bufferConsumer = (Item[]) new Object[buffer.item.length];
                System.arraycopy( buffer.item, 0, bufferConsumer, 0, buffer.item.length );

                // 休眠一下子
                Thread.sleep(150);

                // V 操作
                mutex.release();
                empty.release();

                // 消费
                System.out.println("消费者 " + num + " 当前消费的元素为: buffer[" + out.item + "] = " + data + "\n"
                        + "整个缓冲区元素依次为: ");
                Arrays.stream(bufferConsumer).forEach(num -> System.out.print(num + " "));
                System.out.println("\n----------------------------------------------------------");

                if (count++ >= threshold)
                    break;
            } catch (InterruptedException ie) {
                ie.printStackTrace();
            }
        }
    }
}

/**
 * OS Lab1
 * 使用信号量机制来处理生产者消费者问题
 * @since 1.8
 * @param <Item> 缓冲区元素类型
 */
public class Lab1<Item> {
    // 有界缓冲区大小
    private final static int BufferSIZE = 20;

    @SuppressWarnings("unchecked")
    private void producersConsumers(int bufferSize, Item initValue) {
        // 线程池
        ExecutorService exec = Executors.newCachedThreadPool();

        // 有界缓冲区
        Item[] buffer = (Item[]) new Object[bufferSize];
        for (int i = 0; i < buffer.length; i++) {
            buffer[i] = initValue;
        }

        Container<Item[]> bufferContainer = new Container<>(buffer);

        // 生产者和消费者的 index
        Container<Integer> in = new Container<>(0), out = new Container<>(0);

        // 信号量
        final Semaphore mutex = new Semaphore(1), empty = new Semaphore(bufferSize),
                full = new Semaphore(0);

        // 4 个生产者, 6 个消费者
        List<Integer> queue = Arrays.asList(1, 1, 1, 1, 0, 0, 0, 0, 0, 0);
        Collections.shuffle(queue);
        System.out.println("Indices: ");
        for (Integer i : queue)
            System.out.print((i == 1 ? "生产者 " : "消费者 ") + i + " ");
        System.out.println();
        System.out.println("----------------------------------------------------------");

        for (int j = 0; j < queue.size(); j++) {
            Integer i = queue.get(j);
            if (i == 1) {
                // 生产者
                exec.execute(new Producer<>(bufferContainer, mutex, empty, full, in, j));
            } else {
                // 消费者
                exec.execute(new Consumer<>(bufferContainer, mutex, empty, full, out, j));
            }
        }

        // 退出线程池
        exec.shutdown();
    }

    /**
     * test entry
     * @param args nullable
     */
    public static void main(String[] args) {
        int initValue = 0;
        new Lab1<Integer>().producersConsumers(BufferSIZE, initValue);
    }
}///~
