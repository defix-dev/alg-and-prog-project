package com.sigma_squad.web.services.db.abstractions;

import com.sigma_squad.web.services.token.UserAuthDTO;

public interface IDBSaver {
    void saveUserAuthDTOBySessionId(UserAuthDTO dto, String sessionId);
}
