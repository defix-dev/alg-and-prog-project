package com.sigma_squad.web.services.authorization.abstractions;

import org.springframework.stereotype.Service;

import java.net.http.HttpResponse;

public interface IAuthStrategy {
    HttpResponse<String> authorize(String authType, String token, String sessionId);
}
