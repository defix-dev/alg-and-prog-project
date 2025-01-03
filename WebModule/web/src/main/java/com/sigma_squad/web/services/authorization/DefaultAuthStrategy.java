package com.sigma_squad.web.services.authorization;

import com.sigma_squad.web.services.authorization.abstractions.IAuthStrategy;
import com.sigma_squad.web.services.token.abstractions.JWTDTO;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

@Service
public class DefaultAuthStrategy implements IAuthStrategy {
    private final HttpClient client = HttpClient.newBuilder().build();

    @Override
    public HttpResponse<String> authorize(String authType, String token, String sessionId) {
        return executeRequestToAuthModule(authType, token, sessionId);
    }

    private HttpResponse<String> executeRequestToAuthModule(String authType, String token, String sessionId) {
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create("http://localhost:3031/login?type=" + authType + "&token="+token))
                .POST(HttpRequest.BodyPublishers.noBody())
                .header("Cookie", "JSESSIONID="+sessionId)
                .build();
        try {
            return client.send(request, HttpResponse.BodyHandlers.ofString());
        } catch (IOException | InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
