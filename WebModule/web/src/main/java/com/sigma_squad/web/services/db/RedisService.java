package com.sigma_squad.web.services.db;

import com.sigma_squad.web.services.token.UserAuthDTO;
import com.sigma_squad.web.services.db.abstractions.IDBService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Service
public class RedisService implements IDBService {
    private static final Logger logger = LoggerFactory.getLogger(RedisService.class);

    private final RedisTemplate<String, UserAuthDTO> redisTemplate;

    @Autowired
    public RedisService(RedisTemplate<String, UserAuthDTO> redisTemplate) {
        this.redisTemplate = redisTemplate;
    }

    @Override
    public UserAuthDTO getUserAuthDTOBySessionId(String sessionId) {
        try {
            UserAuthDTO dto = redisTemplate.opsForValue().get(sessionId);
            if (dto == null) {
                logger.warn("No UserAuthDTO found for sessionId: {}", sessionId);
            }
            return dto;
        } catch (Exception e) {
            logger.error("Error occurred while getting UserAuthDTO for sessionId: {}", sessionId, e);
            throw new RuntimeException("Error while accessing Redis", e);
        }
    }

    @Override
    public void saveUserAuthDTOBySessionId(UserAuthDTO dto, String sessionId) {
        if (dto == null) {
            logger.error("Attempted to save null UserAuthDTO for sessionId: {}", sessionId);
            throw new IllegalArgumentException("UserAuthDTO must not be null");
        }

        try {
            redisTemplate.opsForValue().set(sessionId, dto);
        } catch (Exception e) {
            logger.error("Error occurred while saving UserAuthDTO for sessionId: {}", sessionId, e);
            throw new RuntimeException("Error while saving to Redis", e);
        }
    }

    @Override
    public boolean isExistUserAuthDTO(String sessionId) {
        try {
            boolean exists = redisTemplate.hasKey(sessionId);
            if (!exists) {
                logger.warn("No UserAuthDTO found for sessionId: {}", sessionId);
            }
            return exists;
        } catch (Exception e) {
            logger.error("Error occurred while checking existence of UserAuthDTO for sessionId: {}", sessionId, e);
            throw new RuntimeException("Error while checking Redis for sessionId", e);
        }
    }
}
