package org.defix.ddos.executor.abstractions;

import org.defix.proxyDynamicFinder.objects.ProxyBody;

public abstract class DosExecutor {
    protected final ProxyBody proxy;

    public DosExecutor(ProxyBody proxy) {
        this.proxy = proxy;
    }

    public abstract boolean tryExecute();
}
