package org.defix.proxyDynamicFinder.site;

import org.defix.proxyDynamicFinder.objects.ProxyBody;
import org.defix.proxyDynamicFinder.site.abstractions.SiteProxyParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class ProxyCompassSiteProxyParser implements SiteProxyParser {
    private static final Logger logger = LoggerFactory.getLogger(ProxyCompassSiteProxyParser.class);
    private static final String TARGET_URL = "https://proxycompass.com/ru/free-proxy/";
    private String page;

    @Override
    public void update() {
        try {
            HttpClient client = HttpClient.newBuilder().build();
            HttpRequest req = HttpRequest.newBuilder()
                    .GET().uri(URI.create(TARGET_URL)).build();
            page = client.send(req, HttpResponse.BodyHandlers.ofString()).body();
            page = page.substring(page.indexOf("<tr>") + 1);
            page = page.substring(page.indexOf("<tr>"));
            client.close();
        } catch (Exception e) {
            page = "";
            logger.info(STR."Failed to update parser: \{e.getMessage()}");
        }
    }

    @Override
    public ProxyBody parseNext() {
        try {
            page = page.substring(page.indexOf("<td"));
            page = page.substring(page.indexOf(">") + 1);
            String ip = page.substring(0, page.indexOf("<"));
            page = page.substring(page.indexOf("<td>"));
            page = page.substring(page.indexOf(">") + 1);
            int port = Integer.parseInt(page.substring(0, page.indexOf("<")));
            page = page.substring(page.indexOf("<tr>"));
            return new ProxyBody(ip, port);
        } catch (Exception e) {
            logger.error(STR."Failed to parse proxy: \{e.getMessage()}");
            return null; }
    }
}
