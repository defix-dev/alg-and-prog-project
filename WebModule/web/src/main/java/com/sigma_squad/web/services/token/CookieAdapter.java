package com.sigma_squad.web.services.token;

import com.sigma_squad.web.services.token.exceptions.JSESSIONIDDoNotFoundException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Arrays;

@Service
public class CookieAdapter {
    private final HttpSession session;

    @Autowired
    public CookieAdapter(HttpSession session) {
        this.session = session;
    }
}
