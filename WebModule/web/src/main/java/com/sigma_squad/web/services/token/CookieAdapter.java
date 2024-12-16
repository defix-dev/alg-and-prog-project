package com.sigma_squad.web.services.token;

import com.sigma_squad.web.services.token.exceptions.JSESSIONIDDoNotFoundException;
import jakarta.servlet.http.HttpServletRequest;
import org.springframework.stereotype.Service;

import java.util.Arrays;

@Service
public class CookieAdapter {
    private final HttpServletRequest request;

    public CookieAdapter(HttpServletRequest request) {
        this.request = request;
    }

    public String getSessionId() throws JSESSIONIDDoNotFoundException {
        return Arrays
                .stream(request.getCookies())
                .filter(cookie -> cookie.getName().equals("JSESSIONID"))
                .findFirst().orElseThrow(JSESSIONIDDoNotFoundException::new).getValue();
    }
}
