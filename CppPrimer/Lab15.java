import java.sql.*;
import java.util.*;

/**
 * Lab15 JDBC
 * MariaDB JDBC Driver jar包放在该源文件目录, 运行的时候请指定 jar: 
 * java -cp ".:mariadb-java-client-2.2.1.jar" Lab15
 * @author DCMMC
 */
public class Lab15 {
    // The JDBC Connector Class.
    private static final String DBClassName = "org.mariadb.jdbc.Driver";
    //sql 命令
    private static String sql;

    // Connection string. test 是数据库的名称
    // mariaDB是运行在 docker中的, 
    // docker pull mariadb 下载容器, 创建并运行容器(容器中的mariaDB运行在3306端口, 并且绑定到host的3306端口):
    // docker run -p 127.0.0.1:3306:3306 --name mariadbTest -e MYSQL_ROOT_PASSWORD=97294597 -d mariadb
    // docker exec -it mariadbTest bash 进入该容器
    // 进入容器后 mysql -p -u root 连接 mariaDB(密码就是97294597)
    // create database test; 创建测试数据库, exit退出
    // 可以用 docker stop --time=30 mariadbTest 关闭容器(超时时间30), 用 docker rm -v mariadbTest 删除容器
    // 容器一定要记得关闭, 然后以后都使用 dockcer start [container name] 运行 container
    // mysql支持 utf8: 在配置文件(/etc/mysql/my.cnf)中加入:
    // [mysqld]
    // character-set-server=utf8
    // [client]
    // default-character-set=utf8

    private static final String CONNECTION =
                          "jdbc:mariadb://localhost:3306/test?useUnicode=true&characterEncoding=UTF8";
    public static void main(String[] args) {
        // Attempt to load database driver
        try {
            //用于测试 database driver class 是否存在
            Class.forName(DBClassName).newInstance();
            // Properties for user and password. Here the user and password are both 'paulr'
            Properties p = new Properties();
            p.put("user","root");
            p.put("password","97294597");
            // Now try to connect, 一个 Connection代表一个数据库连接
            Connection conn = DriverManager.getConnection(CONNECTION, p);

            /**
             * MySQL(MariaDB) 简明指令 reference
             * 查看:
             * // 查看所有的数据库列表
             * show databases; 
             * // 查看已创建的某一个数据库的信息
             * show create database [数据库名称]; 
             * // 进入数据库
             * use [database name];
             * //查看数据表, 只能在选定进入了数据库才能用, 所有关于数据表的操作都是
             * show tables;
             * //查看数据表信息，后面加上参数\G可使结果更加美观
             * show create table [table name];
             * //查看表的的字段信息
             * desc [table name];
             * 
             * 修改:
             * // 删除数据库
             * drop database [database name]; 
             * // 修改数据库的编码
             * alter database [database name] default character set utf8 collate utf8_general_ci;
             * // 创建数据表, member list为 [Field Type, ...]形式, Type 常用的有 int, varchar, 都可以在类型后面
             * // 用括号限制类型的长度, e.g. int(3)表示整数的长度
             * create table [table name] ([member list]);
             * // 修改表名
             * alter table [old name] rename \[to\] [new name]; //\[ 表示真正在命令中出现的 [
             * //修改字段名
             * alter table [table name] change name [new table name] [type];
             * 
             * // 插入一条数据，也可以少某个字段的同时也少对应的数据, 这些操作都是选定数据库之后才能使用的
             * // varchar用 '' 表示其字面量, 甚至可以省略Fields List, 不过要注意Values的顺序
             * insert into [table name] ([Fields List]) values [ ([Values]), ...];
             * // 更新
             * //更新id=1的数据
             * update [table name] set [Field = Value, ...] where id=1;
             * //批量更新,如果没有where子句，会更新表中所有对应数据
             * update [table name] set [Field = Value, ...] where id < 4;
             * //删除id=6的数据
             * delete from [table name] where id=6;
             * //批量删除数据
             * delete from [table name] where id>3;
             * //删除所有数据,DDL(数据定义语言)语句 truncate table [table name] 也可以删除表内所有数据
             * delete from [table name];
             */

            // Statement里面带有很多方法，比如executeUpdate可以实现插入，更新和删除等
            Statement stmt = conn.createStatement();
            Scanner sc = new Scanner(System.in);
            int cnt = 0;

            // 删除已有的 student 数据表
            sql = "drop table if exists student;";
            stmt.executeUpdate(sql);
            sql = "create table student(id int(9), name varchar(20), chinese int(3), math int(3), "
                + "english int(3));";
            // executeUpdate语句会返回一个受影响的行数，如果返回-1就没有成功
            int result = stmt.executeUpdate(sql);
            
            System.out.println("******************* 利用 MariaDB(MySQL的一个开源分支) 和 JDBC 进行成绩管理的 Demo "
                + " *******************");
            while (true) {
                while (true) {
                    System.out.println("请输入姓名 语文 数学 英语(输入exit() 退出成绩录入)");
                    String name = sc.next();
                    if (name.equals("exit()")) {
                        break;
                    }
                    cnt++;
                    int ch = sc.nextInt(), math = sc.nextInt(), en = sc.nextInt();
                    // 插入数据库
                    sql = "insert into student values( " + cnt + ", '" + name + "' , " + ch + ", " + math + ", "
                        + en + ");";
                    result = stmt.executeUpdate(sql);
                }
                System.out.println("读取完成, 是否查询? (y/n)");
                String answer = sc.next();
                while (answer.startsWith("y")) {
                    // 查询
                    System.out.println("请输入姓名或者输入viewAll()查看所以结果:");
                    String name = sc.next();
                    
                    if (name.equals("viewAll()")) {
                        sql = "select * from student";
                        ResultSet rs = stmt.executeQuery(sql);// executeQuery会返回结果的集合，否则返回空值
                        System.out.println("编号\t姓名\t语文\t数学\t外语");
                        while (rs.next()) {
                            System.out.println(rs.getInt(1) + "\t" + rs.getString(2)
                                + "\t" + rs.getInt(3) + "\t" + rs.getInt(4) + "\t"
                                + rs.getInt(5));
                        }
                    } else {
                        sql = "select * from student where name like \"" + name + "\";";
                        ResultSet rs = stmt.executeQuery(sql);
                        if (!rs.next()) {
                            System.out.println("没有找到有关\"" + name + "\"的成绩信息!");
                        } else {
                            System.out.println("找到有关\"" + name + "\"的成绩信息!");
                            System.out.println("编号\t姓名\t语文\t数学\t外语");
                            System.out.println(rs.getInt(1) + "\t" + rs.getString(2)
                            + "\t" + rs.getInt(3) + "\t" + rs.getInt(4) + "\t"
                            + rs.getInt(5));
                        }
                    }

                    System.out.println(" 是否继续查询? (y/n)");
                    if (!sc.next().startsWith("y")) {
                        break;
                    }
                    answer = "y";
                }
                System.out.println("是否继续导入学生成绩? (y/n)");
                if (!sc.next().startsWith("y")) {
                    System.out.println("退出...");
                    break;
                }
            }

            System.out.println("Close MariaDB Connect !");
            conn.close();
        } catch (ClassNotFoundException cnfe) {
            System.err.println ("Unable to load database driver");
        	System.err.println ("Details : " + cnfe);
        } catch(InstantiationException ie) {
            System.err.println ("Unable to load database driver");
        	System.err.println ("Details : " + ie);
        } catch(IllegalAccessException iae) {
            iae.printStackTrace();
        } catch(SQLException se) {
            System.out.println("MySQL操作错误");
            se.printStackTrace();
        }
    }
}