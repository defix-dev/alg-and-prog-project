package com.sigma_squad.web.authorization;

import lombok.Getter;

@Getter
public enum AuthStatus {
    AUTHORIZED("Authorized"),
    ANONYMOUS("Anonymous");

    private final String statusName;

    AuthStatus(String statusName) {
        this.statusName = statusName;
    }
}