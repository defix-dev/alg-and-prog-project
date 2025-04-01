package org.defix;

import org.defix.ddos.DDosRunner;
import org.defix.proxyDynamicFinder.thread.AsyncSiteProxyFinder;
import org.defix.proxyDynamicFinder.thread.events.OnFindEvent;
import org.slf4j.LoggerFactory;

public class Main {
    public static void main(String[] args) {
        AsyncSiteProxyFinder finder = new AsyncSiteProxyFinder();
        finder.getOnFindEvent().addListener(proxyBody -> {
            LoggerFactory.getLogger(OnFindEvent.class).info(STR."<PROXY FIND> /\{proxyBody.ip()}:\{proxyBody.port()}/");
            return null;
        });
        //finder.getOnFindEvent().addListener(proxyBody -> { DDosRunner.RunDos(proxyBody); return null; });
        finder.findAsync().join();
    }
}