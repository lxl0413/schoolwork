package server;

import java.io.*;
import java.nio.charset.StandardCharsets;

public class Response {
    private static final int BUFFER_SIZE = 1024*8;
    Request request;
    OutputStream outputStream;

    public Response(OutputStream outputStream, Request request) {
        this.outputStream = outputStream;
        this.request = request;
    }

    public void response() throws IOException {
        InputStream textStream = null;
        try {
            String method = request.getMethod();
            String uri = request.getUri();
            if(method.equals("GET")) {
                textStream = this.getClass().getResourceAsStream("/text"+uri);
                System.out.println("/text"+uri);
                if(textStream != null) {
                    doGet(uri, textStream);                     //处理get请求
                }
                else {
                    String errorMessage = "HTTP/1.1 404 File Not Found\r\nContent-Type:text/html\r\nContent-Length:0\r\n\r\n";
                    outputStream.write(errorMessage.getBytes());    //发送出错信息
                }
            }
            else {
                //确认是dopost请求，否则无法处理
                if(uri.equals("/dopost")) {
                    doPost();                                  //处理post请求
                } else {
                    String errorMessage = "HTTP/1.1 404 File Not Found\r\nContent-Type:text/html\r\nContent-Length: 0\r\n\r\n";
                    outputStream.write(errorMessage.getBytes());    //发送出错信息
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        } finally {
            if(textStream != null) {
                textStream.close();
            }
        }
    }

    public void doGet(String uri, InputStream textStream) throws IOException {
        byte[] mainBody = textStream.readAllBytes();

        StringBuilder header = new StringBuilder();
        header.append("HTTP/1.1 200 OK\r\nContent-Type:");

        //设置头部的content-type
        if (uri.contains("html")) {
            header.append("text/html");
        }
        else if(uri.contains("jpg")) {
            header.append("image/jpg");
        }
        else {
            header.append("text/plain");
        }

        header.append("\r\nContent-Length:");
        header.append(mainBody.length);
        header.append("\r\n\r\n");
        byte[] headerBytes = null;
        if(uri.contains("html")) {
            headerBytes = header.toString().getBytes();
        } else {
            headerBytes = header.toString().getBytes(StandardCharsets.UTF_8);
        }
        outputStream.write(headerBytes);
        outputStream.write(mainBody);
    }

    public void doPost() throws IOException {
        StringBuilder header = new StringBuilder();
        header.append("HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nContent-Length:");
        //设置登录结果
        String result = "Login Fail!";
        if(request.checkUser()) {
            result = "Login Success!";
        }
        System.out.println(result);
        //设置响应正文
        String mainBody = "<html><body><h1>" + result + "</h1></body></html>";
        byte[] mainBodyBytes = mainBody.getBytes();
        header.append(mainBodyBytes.length);
        header.append("\r\n\r\n");
        //写入输出流中
        outputStream.write(header.toString().getBytes());
        outputStream.write(mainBodyBytes);
    }
}
