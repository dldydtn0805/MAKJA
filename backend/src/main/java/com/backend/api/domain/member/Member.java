package com.backend.api.domain.member;

import com.backend.api.domain.gameboard.GameBoard;
import jakarta.persistence.*;
import jakarta.validation.constraints.NotNull;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.experimental.SuperBuilder;

import java.util.List;

import static lombok.AccessLevel.PROTECTED;

@Entity
@Getter
@SuperBuilder
@Table(name = "member")
@NoArgsConstructor(access = PROTECTED)
public class Member {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "member_id", nullable = false)
    private Long id;

    @NotNull
    private String nickname;

    @NotNull
    private String password;

    @OneToMany(mappedBy = "member", cascade = CascadeType.ALL)
    private List<GameBoard> gameBoards;

    public Member(Long id, String nickname, String password) {
        this.id = id;
        this.nickname = nickname;
        this.password = password;
    }
}

