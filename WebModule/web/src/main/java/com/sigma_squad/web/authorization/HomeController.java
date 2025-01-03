package com.sigma_squad.web.authorization;

import com.sigma_squad.web.services.token.abstractions.ITokenSaver;
import com.sigma_squad.web.services.token.abstractions.JWTDTO;
import com.sigma_squad.web.services.token.redis.RedisTokenSaver;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import java.util.Arrays;
import java.util.Optional;

@Controller
@RequestMapping("/")
public class HomeController {
    private final ITokenSaver saver;

    @Autowired
    public HomeController(ITokenSaver saver) {
        this.saver = saver;
    }

    @GetMapping
    public String home() {
        return "home";
    }

    @PostMapping
    @ResponseBody
    public ResponseEntity<Void> home(@RequestBody ResponseData data, HttpServletRequest request) {
        Optional<Cookie> sessionCook = Arrays.stream(request.getCookies()).filter(cookie -> cookie.getName().equals("JSESSIONID")).findFirst();
        if(data.getResponseStatus() == HttpStatus.OK.value() && sessionCook.isPresent()) {
            saver.saveJWTTokens(new JWTDTO(
                    data.getRefreshToken(),
                    data.getAccessToken()
            ), sessionCook.get().getValue());
        }
        return ResponseEntity.ok().build();
    }
}
