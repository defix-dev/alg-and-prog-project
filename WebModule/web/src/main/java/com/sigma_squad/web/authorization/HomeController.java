package com.sigma_squad.web.authorization;

import com.sigma_squad.web.services.token.TokenSaver;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
@RequestMapping("/")
public class HomeController {
    private final TokenSaver saver;

    @Autowired
    public HomeController(TokenSaver saver) {
        this.saver = saver;
    }

    @GetMapping
    public String home() {
        return "home";
    }

    @PostMapping
    public ResponseEntity<Void> home(@RequestParam(value = "access_token", required = true) String accessToken,
                                     @RequestParam(value = "refresh_token",required = true) String refreshToken) {
        System.out.println(accessToken);
        System.out.println(refreshToken);
        saver.saveJWTTokens(accessToken, refreshToken);
        return ResponseEntity.ok().build();
    }
}
