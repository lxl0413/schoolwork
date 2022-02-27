package server;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

public class Request {
    private static final int BUFFER_SIZE = 16*1024;
    private InputStream inputStream;
    private String uri;
    private String method;
    private Map<String, String> user;

    public Request(InputStream inputStream) {
        this.inputStream = inputStream;
        uri = null;
        method = null;
    }

    public String getUri() {
        return uri;
    }

    public String getMethod() {
        return method;
    }

    //读取request信息，并设置uri
    public void parse() {
        StringBuffer request = new StringBuffer(BUFFER_SIZE);
        int i = 0;
        byte[] buffer = new byte[BUFFER_SIZE];
        try {
            i = inputStream.read(buffer);
        } catch (IOException e) {
            e.printStackTrace();
            i -= 1;
        }

        for(int j = 0; j < i; j++) {
            request.append((char) buffer[j]);
        }

        //设置uri
        uri = parseUri(request.toString());
    }

    public String parseUri(String requestString) {
        //test if the input contains \r\n\r\n
        if(requestString.indexOf("\r\n\r\n") == -1) {
            return null;
        }
        //从HTML中读出method
        int index1 = 0, index2 = 0;
        index1 = requestString.indexOf(' ');
        method = requestString.substring(0, index1);
        String uri = null;
        if(index1 != -1) {
            index2 = requestString.indexOf(' ',index1+1);
            if(index2 != -1) {
                uri = requestString.substring(index1+1, index2);    //读取uri
            }
        }
        System.out.println("The origin path is: "+uri);
        //处理get请求
        if(method.equals("GET")) {
            //将外部访问地址映射到实际文件实际存放的地址
            if(uri.contains(".html")) {
                uri = "/html" + uri;
            }
            else if(uri.contains(".txt")) {
                uri = "/txt" + uri;
            }
            else {
                uri = "/img" + uri;
            }
            System.out.println("The real path is: "+uri);
        }
        //处理POST请求
        if(method.equals("POST")) {
            //将用户名和密码放到user的Map中
            user = new HashMap<>();
            int loginOffset = requestString.indexOf("login");
            int loginEnd = requestString.indexOf('&', loginOffset+6);
            user.put("login", requestString.substring(loginOffset+6,loginEnd));

            int passOffset = requestString.indexOf("pass");
            int passEnd = requestString.indexOf('&', passOffset+4);
            if(passEnd == -1) {
                passEnd = requestString.length();
            }
            user.put("pass", requestString.substring(passOffset+5,passEnd));
            System.out.println("login="+user.get("login")+" pass="+user.get("pass"));
        }
        return uri;
    }

    //返回登录结果查询值
    public boolean checkUser() {
        if(user.size() == 2) {
            return user.get("login").equals("3190101331") && user.get("pass").equals("1331");
        }
        return false;
    }
}
