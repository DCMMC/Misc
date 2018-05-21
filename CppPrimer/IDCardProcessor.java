import java.sql.ResultSet;
import java.sql.*;
import java.util.*;
import com.monitorjbl.xlsx.*;
import com.monitorjbl.xlsx.impl.*;
import java.io.*;
import org.apache.poi.ss.usermodel.*;
import org.apache.poi.xssf.usermodel.*;
import org.apache.commons.httpclient.Cookie;
import org.apache.commons.httpclient.Header;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.cookie.CookiePolicy;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.methods.PostMethod;
import java.net.*;

/**
 * ############################ 本 class 不能外传 #############################
 * #################### 仅限本人使用, 只作为安全测试用途 ##########################
 */
public class IDCardProcessor {
    private final static String DIR_PATH = "/home/kevin/Documents";
    private static final String CONNECTION = "jdbc:mariadb://localhost:3306/DHU?useUnicode=true&characterEncoding=UTF8";
    // The JDBC Connector Class.
    private static final String DBClassName = "org.mariadb.jdbc.Driver";
    public static void main(String[] args) {
        try {
            //用于测试 database driver class 是否存在
            Class.forName(DBClassName).newInstance();
            // Properties for user and password. Here the user and password are both 'paulr'
            Properties p = new Properties();
            p.put("user", "root");
            p.put("password", "97294597");
            // Now try to connect, 一个 Connection代表一个数据库连接
            Connection conn = DriverManager.getConnection(CONNECTION, p);
            // Statement里面带有很多方法，比如executeUpdate可以实现插入，更新和删除等
            Statement stmt = conn.createStatement();
            //stmt.executeUpdate("drop table if exists DHUStudentInfo;");
            stmt.executeUpdate(
                    "create table if not exists DHUStudentInfo(className varchar(20), studentName varchar(40), srcLocation varchar(20)"
                            + ", idCard varchar(20), nation varchar(20), phone varchar(20), dormInfo varchar(20), "
                            + "extraInfo varchar(4000), studentId varchar(255), UNIQUE (idCard)) DEFAULT CHARSET=utf8;");
            System.out.println("Create table if not exists...");

            //stmt.executeUpdate("delete from DHUStudentInfo where studentName = '李超';");
            //stmt.executeUpdate("insert into DHUStudentInfo VALUES (" +
            //    "'光电1001', '李超', '宁夏', '640203199202110532', '', '', '', '', '' );");
            //stmt.executeUpdate("insert into DHUStudentInfo " +
            //    "(className, studentName, srcLocation, idCard, nation, phone, dormInfo, extraInfo, studentId) " +
            //    "SELECT '光电1001', '李超', '宁夏', '640203199202110532', '', '', '', '', '' " +
            //    "FROM DUAL WHERE NOT EXISTS ( SELECT * FROM DHUStudentInfo WHERE idCard = '430302199806281068' );");

            Scanner sc = new Scanner(System.in);
            System.out.println("是否开始导入学生信息(只用于第一次)? (y/n/help):");
            if (sc.next().startsWith("y")) {
                try (InputStream is = new FileInputStream(new File(DIR_PATH + "/机械1602班信息确认表.xlsx"));
                        Workbook workbook = StreamingReader.builder().rowCacheSize(1000).bufferSize(40960).open(is)) {
                    StringBuilder sql = new StringBuilder("insert into DHUStudentInfo "
                            + "(className, studentName, srcLocation, idCard, nation, phone, dormInfo, extraInfo, studentId) SELECT ");
                    for (Sheet sheet : workbook) {
                        System.out.println("正在处理表格 \"" + sheet.getSheetName() + "\"");
                        for (Row r : sheet) {
                            String idCard = "";
                            for (Cell c : r) {
                                // 判断是否是身份证
                                if (c.getStringCellValue().toString().matches("[0-9xX]{8,}")) {
                                    idCard = c.getStringCellValue();
                                }
                                sql.append("'").append(c.getStringCellValue()).append("',");
                            }
                            // 学号
                            sql.append("'',");
                            sql.delete(sql.length() - 1, sql.length());
                            // 如果不存在, 则插入
                            sql.append(" FROM DUAL WHERE NOT EXISTS ( SELECT * FROM DHUStudentInfo WHERE idCard = '"
                                    + idCard + "');");
                            // 判断是否是表格的第一行
                            if (!(sql.toString().contains("宿舍信息"))) {
                                stmt.executeUpdate(sql.toString());
                            }

                            sql.replace(0, sql.length(), "insert into DHUStudentInfo "
                                    + "(className, studentName, srcLocation, idCard, nation, phone, dormInfo, extraInfo, studentId) SELECT ");
                        }
                    }
                } catch (Exception e) {
                    //...
                    e.printStackTrace();
                }
                System.out.println("读取完成.");
            }

            System.out.println("是否从其他xlsx(不能是 xls )文件中导入学号?(y/n)");
            if (sc.next().equals("y")) {
                System.out.println("输入文件名: (包括后缀)");
                String fName = sc.next();
                if (fName.endsWith(".xlsx")) {
                    File wordFile = new File(DIR_PATH, fName);
                    if (wordFile.exists()) {
                        System.out.println("请分别输出姓名和学号所在列:(从1开始)");
                        int nameRow = sc.nextInt(), studentIdRow = sc.nextInt();

                        try {
                            Workbook workbook = new XSSFWorkbook(wordFile);
                            for (Sheet sheet : workbook) {
                                System.out.println("正在处理表格 \"" + sheet.getSheetName() + "\"");
                                for (Row r : sheet) {
                                    if (Math.max(nameRow, studentIdRow) <= r.getPhysicalNumberOfCells()) {
                                        // 确保单元格类型合法
                                        if (((XSSFCell) r.getCell(nameRow - 1)).getCellTypeEnum() == CellType.STRING
                                                && !(r.getCell(nameRow - 1).getStringCellValue().equals(""))
                                                && (((XSSFCell) r.getCell(studentIdRow - 1))
                                                        .getCellTypeEnum() == CellType.NUMERIC
                                                        || (((XSSFCell) r.getCell(studentIdRow - 1))
                                                                .getCellTypeEnum() == CellType.STRING
                                                                && r.getCell(studentIdRow - 1).getStringCellValue()
                                                                        .matches("\\d{8,}")))) {
                                            String nameStr = r.getCell(nameRow - 1).getStringCellValue(),
                                                    studentIdStr = ((XSSFCell) r.getCell(studentIdRow - 1))
                                                            .getCellTypeEnum() == CellType.NUMERIC
                                                                    ? String.format("%.0f",
                                                                            r.getCell(studentIdRow - 1)
                                                                                    .getNumericCellValue())
                                                                    : r.getCell(studentIdRow - 1).getStringCellValue();
                                            //debug
                                            System.out.println("name: " + nameStr + ", studenId: " + studentIdStr);

                                            //排除所有17级的
                                            if (studentIdStr.startsWith("17")) {
                                                System.out.println("skip 17");
                                                continue;
                                            }

                                            ResultSet rs = stmt.executeQuery(
                                                    "select * from DHUStudentInfo where studentName like \'" + nameStr
                                                            + "\';");

                                            if (rs.next()) {
                                                //debug 
                                                //System.out.println("idCard: " + rs.getString(4));

                                                stmt.executeUpdate(
                                                        "UPDATE DHUStudentInfo SET studentId = '" + studentIdStr
                                                                + "' WHERE idCard LIKE \"" + rs.getString(4) + "\";");
                                            }
                                        }
                                    }
                                }
                            }
                        } catch (Exception e) {
                            //...
                            e.printStackTrace();
                        }
                        System.out.println("导入完成.");
                    } else {
                        System.out.println("文件不存在!");
                    }
                } else {
                    System.out.println("输入文件有误, 必须是 .xlsx 后缀的");
                }
            }

            System.out.println("是否查询? (y/n)");
            String answer = sc.next();
            while (answer.startsWith("y")) {
                // 查询
                System.out.println("请输入姓名查看所以结果:");
                String name = sc.next();

                String sql = "select * from DHUStudentInfo where studentName like \"" + name + "\";";
                ResultSet rs = stmt.executeQuery(sql.toString());
                if (!rs.next()) {
                    System.out.println("没有找到有关\"" + name + "\"的信息!");
                } else {
                    System.out.println("找到有关\"" + name + "\"的信息!");
                    System.out.println("index\t班级\t姓名\t户籍来源\t身份证编号\t民族\t联系方式\t宿舍信息\t备注\t学号");
                    int cnt = 1;
                    List<String> idCards = new ArrayList<>();
                    do {
                        System.out.print(cnt + "\t");
                        cnt++;
                        System.out.println(rs.getString(1) + "\t" + rs.getString(2) + "\t" + rs.getString(3) + "\t"
                                + rs.getString(4) + "\t" + rs.getString(5) + "\t" + rs.getString(6) + "\t"
                                + rs.getString(7) + "\t" + rs.getString(8) + "\t" + rs.getString(9) + "\n");
                        idCards.add(rs.getString(4));
                    } while (rs.next());

                    System.out.println("是否更改更新当前学生的学号?(y/n)");
                    if (sc.next().equals("y")) {
                        System.out.println("输入序号和学号:");
                        int index = sc.nextInt();
                        String idStr = sc.next();
                        if (idStr.matches("[0-9]{8,}")) {
                            stmt.executeUpdate("UPDATE DHUStudentInfo SET studentId = '" + idStr
                                    + "' WHERE idCard LIKE \"" + idCards.get(index - 1) + "\";");
                        } else {
                            System.out.println("学号有误!");
                        }

                    }
                }

                System.out.println("是否继续查询?(y/n)");
                answer = sc.next();
            }

            //统计有学号的人数
            ResultSet rs = stmt.executeQuery("select count(*) from DHUStudentInfo where studentId != \"\";");
            if (rs.next()) {
                System.out.println("当前已有 " + rs.getString(1) + " 个学号.");
            }

            //爬虫
            System.out.println("是否开始爬取http://tuanyuanzheng.shyouth.net? (y/n)");
            if (sc.next().startsWith("y")) {
                rs = stmt.executeQuery("select * from DHUStudentInfo where studentId != \"\";");
                HttpClient httpClient = new HttpClient();
                // 设置 HttpClient 接收 Cookie,用与浏览器一样的策略
                httpClient.getParams().setCookiePolicy(CookiePolicy.BROWSER_COMPATIBILITY);

                //模拟 Get 登录
                GetMethod getMethod = null;
                while(rs.next()) {
                    String userCookie = "";
                    try {
                        getMethod = new GetMethod("http://tuanyuanzheng.shyouth.net/login_mobile.jsp?flag=1"
                            + "&username=" + URLEncoder.encode(rs.getString(2), "utf-8") + "&idcard=" + rs.getString(4));
                        httpClient.executeMethod(getMethod);
                        //debug
                        //String debug = getMethod.getResponseBodyAsString();
                        //System.out.println(debug);

                        //获得登陆后的Cookie
                        Cookie[] cookies = httpClient.getState().getCookies();
                        for (Cookie c : cookies)
                            userCookie += c.toString() + ";";
    
                        BufferedInputStream bis = new BufferedInputStream(getMethod.getResponseBodyAsStream());
                        Scanner scanner = new Scanner(bis);
                        boolean errFlag = false;
                        String line;
                        while (scanner.hasNextLine()) {
                            //成功
                            if ((line = scanner.nextLine()).contains("我的电子团员证"))
                                break;
    
                            //失败
                            if (line.contains("团员报到")) {
                                errFlag = true;
                            }
                        }

                        if (!errFlag) {
                            // 团员信息维护
                            getMethod = new GetMethod("http://tuanyuanzheng.shyouth.net" + 
                                "/SpesDispatchAction.do?efFormEname=TYWHMB01&serviceName=TJMem" +
                                "berInfo&methodName=queryMemberBySessionEmpCode");
                            getMethod.setRequestHeader("cookie", userCookie);

                            //提交请求
                            httpClient.executeMethod(getMethod);

                            //debug
                            String debug2 = getMethod.getResponseBodyAsString();
                            System.out.println(debug2);


                        } else {
                            System.out.println("姓名或身份证号错误.");
                        }

                        System.out.println("是否继续?(y/n)");
                        if (!sc.next().startsWith("y")) {
                            System.out.println("break");
                            break;
                        }
                    } catch (IOException ioe) {
                        //登录post的时候发生IO异常
                        System.out.println("登录post的时候发生IO异常");
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }

            System.out.println("Close MariaDB Connect !");
            conn.close();
        } catch (ClassNotFoundException cnfe) {
            System.err.println("Unable to load database driver");
            System.err.println("Details : " + cnfe);
        } catch (InstantiationException ie) {
            System.err.println("Unable to load database driver");
            System.err.println("Details : " + ie);
        } catch (IllegalAccessException iae) {
            iae.printStackTrace();
        } catch (SQLException se) {
            System.out.println("MySQL操作错误");
            se.printStackTrace();
        }

    }
}///~
