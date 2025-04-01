package org.defix.proxyDynamicFinder.site.abstractions;

import org.defix.proxyDynamicFinder.objects.ProxyBody;

public interface SiteProxyParser {
    void update();
    ProxyBody parseNext();
}
