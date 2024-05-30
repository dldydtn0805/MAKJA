package com.backend.api.global.security.service;

import com.backend.api.domain.member.Member;
import com.backend.api.domain.member.MemberRepository;
import com.backend.api.global.exception.UserNotFoundException;
import com.backend.api.global.security.SecurityMember;
import lombok.RequiredArgsConstructor;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.stereotype.Service;

@Service("memberDetailsService")
@RequiredArgsConstructor
public class CustomMemberDetailsService implements UserDetailsService {

    private final MemberRepository memberRepository;

    @Override
    public UserDetails loadUserByUsername(String nickname) {
        Member member = memberRepository.findMemberByNickname(nickname)
                .orElseThrow(() -> new UserNotFoundException("토큰에 저장된 아이디가 잘못되었습니다."));
        return new SecurityMember(member);
    }
}
