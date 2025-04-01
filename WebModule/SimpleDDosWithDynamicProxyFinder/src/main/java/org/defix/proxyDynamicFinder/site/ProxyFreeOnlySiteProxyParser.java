package org.defix.proxyDynamicFinder.site;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.defix.proxyDynamicFinder.objects.ProxyBody;
import org.defix.proxyDynamicFinder.site.abstractions.SiteProxyParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.ArrayList;
import java.util.List;

public class ProxyFreeOnlySiteProxyParser implements SiteProxyParser {
    private static final Logger logger = LoggerFactory.getLogger(ProxyFreeOnlySiteProxyParser.class);
    private static final int ATTEMPTS_LIMIT = 5;
    private int pageId = 1;
    private int elementId = 1;
    private List<ProxyBody> proxies = new ArrayList<>();
    private int updateAttemptId = 0;
    private String response;

    @Override
    public void update() {
        elementId = 1;
        proxies = new ArrayList<>();
        try {
            HttpClient client = HttpClient.newBuilder().build();
            HttpRequest req = HttpRequest.newBuilder()
                    .GET().uri(URI.create(getTargetUrl())).build();
            response = client.send(req, HttpResponse.BodyHandlers.ofString()).body();
            pageId++;
            client.close();
        } catch (Exception e) {
            updateAttemptId++;
            response = "";
            logger.error(STR."Failed to update parser: \{e.getMessage()}/AID:\{updateAttemptId}/PID:\{pageId}/URL:\{getTargetUrl()}");
            if(updateAttemptId < ATTEMPTS_LIMIT) update();
        }
    }

    @Override
    public ProxyBody parseNext() {
        try {
            if(proxies.isEmpty()) {
                ObjectMapper mapper = new ObjectMapper();
                JsonNode rootNode = mapper.readTree(response);
                for(JsonNode item : rootNode) {
                    proxies.add(new ProxyBody(item.get("ip").asText(), Integer.parseInt(item.get("port").asText())));
                }
            }
            try {
                return proxies.get(elementId++);
            } catch (Exception _) {
                update();
                return parseNext();
            }
        } catch (Exception e) {
            logger.error(STR."Failed to parse proxy: \{e.getMessage()}");
            return null;
        }
    }

    private String getTargetUrl() {
        return STR."https://proxyfreeonly.com/api/free-proxy-list?limit=50&page=\{pageId}&sortBy=lastChecked&sortType=desc";
    }
}
