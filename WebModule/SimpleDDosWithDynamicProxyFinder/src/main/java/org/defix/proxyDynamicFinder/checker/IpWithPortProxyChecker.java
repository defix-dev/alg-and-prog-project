package org.defix.proxyDynamicFinder.checker;

import org.defix.proxyDynamicFinder.checker.abstractions.ProxyChecker;
import org.defix.proxyDynamicFinder.objects.ProxyBody;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.net.*;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class IpWithPortProxyChecker implements ProxyChecker {
    private static final int MAX_TIMEOUT = 2000;
    private static final String TARGET_REQUESTS_URL = "http://example.com/";
    private static final HttpRequest TARGET_REQUEST = HttpRequest.newBuilder().GET().uri(URI.create(TARGET_REQUESTS_URL)).build();
    private static final Logger logger = LoggerFactory.getLogger(IpWithPortProxyChecker.class);

    @Override
    public boolean check(String ip, int port) {
        return checkIpAndPort(ip, port) && checkRequests(ip, port);
    }

    @Override
    public boolean check(ProxyBody proxy) {
        return check(proxy.ip(), proxy.port());
    }

    private boolean checkIp(String ip) {
        try {
            boolean isWork = InetAddress.getByName(ip).isReachable(MAX_TIMEOUT);
            logger.debug(STR."\{ip} work status: \{isWork}");
            return  isWork;
        } catch (Exception e) { logger.error(STR."Failed to check ip: \{e.getMessage()}"); return false; }
    }

    private boolean checkIpAndPort(String ip, int port) {
        try(Socket socket = new Socket()) {
            socket.connect(new InetSocketAddress(ip, port), MAX_TIMEOUT);
            logger.debug(STR."\{ip}:\{port} work status: true");
            return true;
        } catch (Exception e) { logger.error(STR."Failed to check ip and port: \{e.getMessage()}"); return false; }
    }

    private boolean checkRequests(String ip, int port) {
        try {
            HttpClient client = HttpClient.newBuilder().proxy(
                    ProxySelector.of(new InetSocketAddress(ip, port))
            )
                    .version(HttpClient.Version.HTTP_1_1).build();
            HttpResponse<String> res = client.send(TARGET_REQUEST, HttpResponse.BodyHandlers.ofString());
            client.close();
            logger.debug(STR."Proxy \{ip}:\{port} has been handled a requests");
            return res.body() != null && res.statusCode() == 200;
        } catch (Exception e) { logger.error(STR."Failed to check requests: \{e.getMessage()}"); return false; }
    }
}
