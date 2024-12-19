package com.sigma_squad.web.authorization;

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
    @GetMapping
    public String home() {
        return "home";
    }

    @PostMapping
    public ResponseEntity<Void> home(@RequestParam(value = "access_token", required = true) String accessToken,
                                     @RequestParam(value = "refresh_token",required = true) String refreshToken) {
        System.out.println(accessToken);
        System.out.println(refreshToken);
        return ResponseEntity.ok().build();
    }
}
