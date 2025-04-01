package org.defix.proxyDynamicFinder.checker.abstractions;

import org.defix.proxyDynamicFinder.objects.ProxyBody;

public interface ProxyChecker {
    boolean check(ProxyBody proxy);
    boolean check(String ip, int port);
}
