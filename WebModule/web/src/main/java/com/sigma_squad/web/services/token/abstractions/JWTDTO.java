package com.sigma_squad.web.services.token.abstractions;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class JWTDTO {
    private String refreshToken;
    private String accessToken;
}
