// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus.h"
#include "../Player/Magician/MagicianCharacter.h"
#include "../Player/Magician/GameEndWidget.h"
#include "Components/WidgetComponent.h"
#include "NexusHpBar.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANexus::ANexus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NexusMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NexusMeshComp"));
	SetRootComponent(NexusMeshComp);

	NexusHpBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("NexusHpBarComp"));

	static ConstructorHelpers::FClassFinder<UNexusHpBar> NHB(TEXT("WidgetBlueprint'/Game/UI/Nexus/WBP_NexusHpBar.WBP_NexusHpBar_C'"));
	if (NHB.Succeeded()) {
		NexusHpBarComp->SetWidgetClass(NHB.Class);
	}
}

// Called when the game starts or when spawned
void ANexus::BeginPlay()
{
	Super::BeginPlay();

	UNexusHpBar* NexusHpBar = Cast<UNexusHpBar>(NexusHpBarComp->GetWidget());
	Hp = MaxHp; // 체력 초기화
	NexusHpBar->SetPercentage(100.f * Hp / MaxHp); // 위젯에 반영
	NexusHpBar->AddToViewport(); // 위젯을 월드에 띄운다.

	// 테스트용
	/*FTimerHandle TestHandle;
	GetWorld()->GetTimerManager().SetTimer(TestHandle, this, &ANexus::Test, 0.5f, true);*/
}

//void ANexus::Test() {
//	TakeDamage(57843);
//}

// Called every frame
void ANexus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ANexus::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Hp <= 0) return 0.0f;

	float ret = 0.0f;
	UNexusHpBar* NexusHpBar = Cast<UNexusHpBar>(NexusHpBarComp->GetWidget());
	Hp -= DamageAmount;
	ret = DamageAmount;

	UE_LOG(LogTemp, Log, TEXT("Nexus Hp : %d"), Hp)

	if(Hp <= 0) {
		ret += Hp;
		Hp = 0;
		GameOver();
	}

	// 체력 바 업데이트
	NexusHpBar->SetPercentage(100.f * Hp / MaxHp);
	return ret;
}

void ANexus::GameOver()
{
	AMagicianCharacter* MagicianCharacter = Cast<AMagicianCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MagicianCharacter->GameEndWidget->PostRankingList();
	MagicianCharacter->GameEndWidget->GetRankingList();
	MagicianCharacter->GameEndWidget->SetVisibility(ESlateVisibility::Visible);
}