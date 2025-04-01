package org.defix;

import java.util.concurrent.atomic.AtomicInteger;

public class MySemaphore {
    private final int maxRunningCount;
    private final AtomicInteger currentRunningCount = new AtomicInteger(0);

    public MySemaphore(int maxRunningCount) {
        this.maxRunningCount = maxRunningCount;
    }

    public synchronized void acquire() throws InterruptedException {
        while (currentRunningCount.get() >= maxRunningCount) {
            Thread.currentThread().
            wait(); // Ожидание, если количество текущих потоков больше или равно максимально допустимому
        }
        currentRunningCount.incrementAndGet(); // Увеличиваем количество работающих потоков
    }

    public synchronized void release() {
        if (currentRunningCount.get() > 0) {
            currentRunningCount.decrementAndGet(); // Уменьшаем количество работающих потоков
            notify(); // Уведомляем ожидающие потоки, что один поток освободил ресурс
        }
    }
}
