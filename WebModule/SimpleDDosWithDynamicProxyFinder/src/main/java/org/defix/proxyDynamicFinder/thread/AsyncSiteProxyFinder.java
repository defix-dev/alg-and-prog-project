package org.defix.proxyDynamicFinder.thread;

import org.defix.proxyDynamicFinder.ProxyDynamicFinderConfiguration;
import org.defix.proxyDynamicFinder.objects.ProxyBody;
import org.defix.proxyDynamicFinder.site.abstractions.SiteProxyParser;
import org.defix.proxyDynamicFinder.thread.events.OnFindEvent;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.concurrent.CompletableFuture;

public class AsyncSiteProxyFinder {
    private static final Logger logger = LoggerFactory.getLogger(AsyncSiteProxyFinder.class);
    private final OnFindEvent onFind = new OnFindEvent();

    public CompletableFuture<Void> findAsync() {
        return CompletableFuture.runAsync(() -> {
            try {
                while (true) {
                    for (SiteProxyParser parser : ProxyDynamicFinderConfiguration
                            .proxyParsers) {
                        ProxyBody proxy;
                        parser.update();

                        while ((proxy = parser.parseNext()) != null) {
                            logger.debug(STR."Found proxy: \{proxy.ip()}:\{proxy.port()}");
                            if (ProxyDynamicFinderConfiguration.proxyChecker.check(proxy)) {
                                logger.debug(STR."Found a new proxy: \{proxy.ip()}:\{proxy.port()}");
                                onFind.invoke(proxy);
                            }
                        }
                    }
                }
            } catch (Exception e) {
                logger.error(STR."Failed to find async: \{e.getMessage()}");
                Thread.currentThread().interrupt();
            }
        });
    }

    public OnFindEvent getOnFindEvent() {
        return onFind;
    }
}
