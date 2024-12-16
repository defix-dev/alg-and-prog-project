package com.sigma_squad.web.authorization;

import com.sigma_squad.web.services.token.TokenAdapter;
import com.sigma_squad.web.services.token.TokenGenerator;
import com.sigma_squad.web.services.token.TokenSaver;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicReference;

@Controller
@RequestMapping("/login")
public class AuthorizationController {
    private final TokenSaver saver;
    private final TokenAdapter tokenAdapter;

    @Autowired
    public AuthorizationController(TokenSaver saver, TokenAdapter tokenAdapter) {
        this.saver = saver;
        this.tokenAdapter = tokenAdapter;
    }

    @GetMapping
    public String loginWithoutParams() {
        return "login";
    }

    @Async
    @PostMapping
    public CompletableFuture<ResponseEntity<Void>> loginWithParams(@RequestParam("type") String authType) throws IOException, InterruptedException {
        return CompletableFuture.supplyAsync(() -> {
            HttpClient client = HttpClient.newBuilder().build();
            String token;
            if (tokenAdapter.isExistTokenBody())
                token = tokenAdapter.getTokenBody().getToken();
            else {
                token = TokenGenerator.generateSecureToken();
                saver.saveToken(token);
            }
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create("http://localhost:3031/login".concat("?type=").concat(authType).concat("&token=").concat(token)))
                    .POST(HttpRequest.BodyPublishers.noBody())
                    .build();
            HttpResponse<String> response;
            try {
                response = client.send(request, HttpResponse.BodyHandlers.ofString());
            } catch (IOException | InterruptedException e) {
                throw new RuntimeException(e);
            }
            return ResponseEntity.status(HttpStatus.FOUND).header("Location", response.headers().firstValue("Location").orElse("http://localhost:3030/")).build();
        });
    }
}
