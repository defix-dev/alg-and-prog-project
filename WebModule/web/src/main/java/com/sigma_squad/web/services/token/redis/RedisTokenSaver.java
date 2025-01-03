package com.sigma_squad.web.services.token.redis;

import com.sigma_squad.web.authorization.AuthStatus;
import com.sigma_squad.web.services.db.abstractions.IDBService;
import com.sigma_squad.web.services.token.UserAuthDTO;
import com.sigma_squad.web.services.token.abstractions.ITokenSaver;
import com.sigma_squad.web.services.token.abstractions.JWTDTO;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class RedisTokenSaver implements ITokenSaver {
    private static final Logger logger = LoggerFactory.getLogger(RedisTokenSaver.class);
    private final IDBService dbService;

    @Autowired
    public RedisTokenSaver(IDBService dbService) {
        this.dbService = dbService;
    }

    @Override
    public void saveAuthToken(String token, String sessionId) {
        if (token == null || sessionId == null) {
            logger.error("AuthToken or sessionId is null");
            throw new IllegalArgumentException("AuthToken and sessionId must not be null");
        }
        saveTokens(token, null, sessionId);
    }

    @Override
    public void saveJWTTokens(JWTDTO jwtdto, String sessionId) {
        if (jwtdto == null || sessionId == null) {
            logger.error("JWTDTO or sessionId is null");
            throw new IllegalArgumentException("JWTDTO and sessionId must not be null");
        }
        saveTokens("", jwtdto, sessionId);
    }

    private void saveTokens(String authToken, JWTDTO jwtdto, String sessionId) {
        try {
            boolean authorized = jwtdto != null && !jwtdto.getAccessToken().isEmpty();
            UserAuthDTO dto = createUserAuthDTO(authToken, jwtdto, authorized);
            dbService.saveUserAuthDTOBySessionId(dto, sessionId);
        } catch (Exception e) {
            logger.error("Error occurred while saving tokens for sessionId: {}", sessionId, e);
            throw new RuntimeException("Error while saving tokens", e);
        }
    }

    private UserAuthDTO createUserAuthDTO(String authToken, JWTDTO jwtdto, boolean authorized) {
        UserAuthDTO dto = new UserAuthDTO();
        dto.setAuthToken(authToken);

        if (jwtdto != null) {
            dto.setAccessToken(jwtdto.getAccessToken());
            dto.setRefreshToken(jwtdto.getRefreshToken());
        } else if (authorized) {
            dto.setAccessToken("");
            dto.setRefreshToken("");
        }

        dto.setStatus(authorized ? AuthStatus.AUTHORIZED.getStatusName() : AuthStatus.ANONYMOUS.getStatusName());
        return dto;
    }
}
