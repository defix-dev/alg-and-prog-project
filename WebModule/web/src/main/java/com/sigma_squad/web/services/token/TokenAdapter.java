package com.sigma_squad.web.services.token;

import com.sigma_squad.web.services.token.exceptions.TokenBodyDoNotFoundException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.Map;

@Service
public class TokenAdapter {
    private final RedisTemplate<String, RedisTokenDTO> redis;
    private final CookieAdapter cookie;

    @Autowired
    public TokenAdapter(RedisTemplate<String, RedisTokenDTO> redis, CookieAdapter cookie) {
        this.redis = redis;
        this.cookie = cookie;
    }

    public RedisTokenDTO getTokenBody() throws TokenBodyDoNotFoundException {
        if(!isExistTokenBody()) throw new TokenBodyDoNotFoundException();
        return redis.opsForValue().get(cookie.getSessionId());
    }

    public boolean isExistTokenBody() {
        return redis.hasKey(cookie.getSessionId());
    }
}
