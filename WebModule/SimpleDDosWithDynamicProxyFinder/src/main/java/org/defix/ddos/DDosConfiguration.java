package org.defix.ddos;

import org.defix.ddos.executor.HttpDosExecutor;
import org.defix.ddos.executor.abstractions.DosExecutor;
import org.defix.proxyDynamicFinder.objects.ProxyBody;

public class DDosConfiguration {
    public static final String TARGET_SERVER = "http://example.com/";
    public static final byte THREAD_COUNT = 10;
    public static final byte THREAD_CACHED_COUNT = 5;

    public static DosExecutor getDosExecutor(ProxyBody proxy) {
        return new HttpDosExecutor(proxy);
    }
}
