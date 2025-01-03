package com.sigma_squad.web.services.token.redis;

import com.sigma_squad.web.services.db.abstractions.IDBService;
import com.sigma_squad.web.services.token.UserAuthDTO;
import com.sigma_squad.web.services.token.abstractions.ITokenAdapter;
import com.sigma_squad.web.services.token.exceptions.TokenBodyDoNotFoundException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class RedisTokenAdapter implements ITokenAdapter {
    private static final Logger logger = LoggerFactory.getLogger(RedisTokenAdapter.class);
    private final IDBService idbService;

    @Autowired
    public RedisTokenAdapter(IDBService idbService) {
        this.idbService = idbService;
    }

    @Override
    public UserAuthDTO getTokenBody(String sessionId) throws TokenBodyDoNotFoundException {
        if(sessionId == null) {
            logger.error("SessionId cannot be null!");
            throw new TokenBodyDoNotFoundException();
        }
        if(idbService.isExistUserAuthDTO(sessionId)) {
            return idbService.getUserAuthDTOBySessionId(sessionId);
        }
        logger.error("Token body with key "+sessionId+" do not found!");
        throw new TokenBodyDoNotFoundException();
    }
}
