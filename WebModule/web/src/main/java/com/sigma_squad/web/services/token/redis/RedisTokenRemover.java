package com.sigma_squad.web.services.token.redis;

import com.sigma_squad.web.services.token.UserAuthDTO;
import com.sigma_squad.web.services.token.abstractions.ITokenRemover;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;

@Service
public class RedisTokenRemover implements ITokenRemover {
    private final RedisTemplate<String, UserAuthDTO> redisTemplate;

    @Autowired
    public RedisTokenRemover(RedisTemplate<String, UserAuthDTO> redisTemplate) {
        this.redisTemplate = redisTemplate;
    }

    @Override
    public void removeTokens(String sessionId) {
        redisTemplate.delete(sessionId);
    }
}
