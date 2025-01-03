package com.sigma_squad.web.services.db.abstractions;

import com.sigma_squad.web.services.token.UserAuthDTO;

public interface IDBAdapter {
    UserAuthDTO getUserAuthDTOBySessionId(String sessionId);
}
