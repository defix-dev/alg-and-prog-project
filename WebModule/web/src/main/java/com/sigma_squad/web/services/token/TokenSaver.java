package com.sigma_squad.web.services.token;

import com.sigma_squad.web.services.token.exceptions.JSESSIONIDDoNotFoundException;
import com.sigma_squad.web.services.token.exceptions.TokenBodyDoNotFoundException;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletRequest;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;

import java.util.Arrays;
import java.util.Map;
import java.util.Optional;

@Service
public class TokenSaver {
    private final RedisTemplate<String, RedisTokenDTO> redis;
    private final TokenAdapter adapter;
    private final CookieAdapter cookie;

    @Autowired
    public TokenSaver(RedisTemplate<String, RedisTokenDTO> redis, TokenAdapter adapter,
                      CookieAdapter cookie) {
        this.redis = redis;
        this.adapter = adapter;
        this.cookie = cookie;
    }

    public void saveToken(String token) {
        if(adapter.isExistTokenBody()) return;

        RedisTokenDTO dto = new RedisTokenDTO();
        dto.setToken(token);
        dto.setStatus("Unrealized");

        redis.opsForValue().set(cookie.getSessionId(), dto);
    }
}
