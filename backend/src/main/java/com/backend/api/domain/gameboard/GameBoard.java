package com.backend.api.domain.gameboard;

import com.backend.api.domain.BaseEntity;
import com.backend.api.domain.member.Member;
import com.backend.api.global.common.type.ModeType;
import com.backend.api.global.common.type.StatusType;
import jakarta.persistence.*;
import jakarta.validation.constraints.NotNull;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import lombok.experimental.SuperBuilder;

import java.time.Duration;
import java.time.LocalDateTime;

import static lombok.AccessLevel.PROTECTED;

@Entity
@Getter
@Setter
@SuperBuilder
@Table(name = "game_board")
@NoArgsConstructor(access = PROTECTED)
public class GameBoard extends BaseEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "game_id", updatable = false, nullable = false)
    private Long id;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "member_id")
    private Member member;

    @NotNull
    private ModeType mode;

    @NotNull
    private Long score;

    @NotNull
    private Long playtime;

    @NotNull
    private StatusType status;

    public void updateGameBoard(Long score, StatusType status) {
        this.status = status;
        this.score = score;
        Duration playtime = Duration.between(this.getStartDate(), LocalDateTime.now());
        this.playtime = playtime.getSeconds();
    }
}
