package server;

import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class HttpServer {
    public static final String WEB_ROOT = System.getProperty("user.dir") + File.separator + "src" + File.separator+ "text";
    public static void main(String[] args) throws IOException {
        HttpServer httpServer = new HttpServer();
        httpServer.await();
    }

    //等待、处理新的套接字连接
    public void await() throws IOException {
        System.out.println("wait for a connection...");
        ServerSocket serverSocket = null;
        int port = 1331;
        try {
            serverSocket = new ServerSocket(port, 1, InetAddress.getByName("127.0.0.1"));
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }
        boolean QUIT = false;
        while(!QUIT) {
            Socket socket = null;
            InputStream inputStream = null;
            OutputStream outputStream = null;
            //循环等待新的连接请求
            try {
                socket = serverSocket.accept();
                inputStream = socket.getInputStream();
                outputStream = socket.getOutputStream();
                //处理请求
                Request request = new Request(inputStream);
                request.parse();
                //响应请求
                Response response = new Response(outputStream, request);
                response.response();
                //检测到退出信号，退出while循环
                if(request.getUri().equals("/html/Quit.html")) {
                    QUIT = true;
                }
            } catch (Exception e) {
                e.printStackTrace();
                continue;
            } finally {
                //关闭socket
                if(socket != null) {
                    socket.close();
                }
            }
        }
    }
}
