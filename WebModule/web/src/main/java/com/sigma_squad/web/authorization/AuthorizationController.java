package com.sigma_squad.web.authorization;

import com.sigma_squad.web.services.token.TokenAdapter;
import com.sigma_squad.web.services.token.TokenGenerator;
import com.sigma_squad.web.services.token.TokenSaver;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpSession;
import lombok.Getter;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

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

    @Getter
    public enum AuthStatus {
        AUTHORIZED("Authorized"),
        ANONYMOUS("Anonymous");

       private final String statusName;

       AuthStatus(String statusName) {
           this.statusName = statusName;
       }
    }

    @Autowired
    public AuthorizationController(TokenSaver saver, TokenAdapter tokenAdapter) {
        this.saver = saver;
        this.tokenAdapter = tokenAdapter;
    }

    @GetMapping
    public String loginWithoutParams() {
        return "login";
    }

    @PostMapping
    public ResponseEntity<Void> loginWithParams(@RequestParam("type") String authType, HttpServletRequest req) {
        HttpSession session = req.getSession(false);
        if(session != null) {
            session.invalidate();
        }
        session = req.getSession(true);

        String token = tokenAdapter.isExistTokenBody()
                ? tokenAdapter.getTokenBody().getAuthToken()
                : TokenGenerator.generateSecureToken();

        saver.saveAuthToken(token);
        HttpClient client = HttpClient.newBuilder().build();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create("http://localhost:3031/login?type=" + authType + "&token="+token))
                .POST(HttpRequest.BodyPublishers.noBody())
                .build();
        HttpResponse<String> response;
        try {
            response = client.send(request, HttpResponse.BodyHandlers.ofString());
        } catch (IOException | InterruptedException e) {
            throw new RuntimeException(e);
        }
        return ResponseEntity.status(HttpStatus.FOUND)
                .header("Location", response.headers().firstValue("Location").orElse("http://localhost:3030/"))
                .build();
    }
}
