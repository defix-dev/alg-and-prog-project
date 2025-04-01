package org.defix.proxyDynamicFinder;

import org.defix.proxyDynamicFinder.checker.IpWithPortProxyChecker;
import org.defix.proxyDynamicFinder.checker.abstractions.ProxyChecker;
import org.defix.proxyDynamicFinder.site.ProxyCompassSiteProxyParser;
import org.defix.proxyDynamicFinder.site.ProxyFreeOnlySiteProxyParser;
import org.defix.proxyDynamicFinder.site.abstractions.SiteProxyParser;

import java.util.List;

public class ProxyDynamicFinderConfiguration {
    public static final List<SiteProxyParser> proxyParsers = List.of(
            new ProxyCompassSiteProxyParser()
    );
    public static final ProxyChecker proxyChecker = new IpWithPortProxyChecker();
}
