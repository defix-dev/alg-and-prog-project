package com.sigma_squad.web.authorization;

import com.sigma_squad.web.services.authorization.AuthorizationService;
import com.sigma_squad.web.services.token.UserAuthDTO;
import com.sigma_squad.web.services.token.abstractions.ITokenAdapter;
import com.sigma_squad.web.services.token.redis.RedisTokenAdapter;
import com.sigma_squad.web.services.token.redis.RedisTokenGenerator;
import com.sigma_squad.web.services.token.redis.RedisTokenSaver;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

@Controller
@RequestMapping("/login")
public class AuthorizationController {
    private final AuthorizationService authService;
    private final ITokenAdapter tokenAdapter;

    @Autowired
    public AuthorizationController(AuthorizationService authService, ITokenAdapter tokenAdapter) {
        this.authService = authService;
        this.tokenAdapter = tokenAdapter;
    }

    @GetMapping
    public String loginWithoutParams(HttpSession session) {
        return !authService.isAuthorize(session.getId()) ? "login" : "redirect:/";
    }

    @PostMapping
    public ResponseEntity<Void> loginWithParams(@RequestParam("type") String authType, HttpServletRequest req) {
        HttpSession session = req.getSession(false);
        if(session != null) {
            session.invalidate();
        }
        session = req.getSession(true);

        HttpResponse<String> response = authService.authorize(authType, session.getId());
        return ResponseEntity.status(HttpStatus.FOUND)
                .header("Location", response.headers().firstValue("Location").orElse("http://localhost:3030/"))
                .build();
    }

    @PostMapping("/logout")
    public ResponseEntity<Void> logout(@RequestParam(required = false) boolean all, HttpServletRequest request) throws IOException, InterruptedException {
        String sessionId = request.getSession().getId();
        authService.logout(sessionId);
        if (all) {
            HttpClient client = HttpClient.newBuilder().build();
            HttpRequest req = HttpRequest.newBuilder().uri(URI.create("http://localhost:3031/logout?refresh_token="+tokenAdapter.getTokenBody(sessionId).getRefreshToken())).POST(HttpRequest.BodyPublishers.noBody()).build();
            client.send(req, HttpResponse.BodyHandlers.ofString());
        }
        return ResponseEntity.status(HttpStatus.FOUND).header("Location", "http://localhost:3030/").build();
    }
}