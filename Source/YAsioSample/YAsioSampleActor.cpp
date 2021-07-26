// Fill out your copyright notice in the Description page of Project Settings.


#include "YAsioSampleActor.h"

#include "yasio/platform/yasio_unreal.hpp"
#include "yasio/yasio.hpp"
#include "yasio/obstream.hpp"

using namespace yasio;
using namespace yasio::inet;

DECLARE_LOG_CATEGORY_EXTERN(yasio_ue4, Log, All);
DEFINE_LOG_CATEGORY(yasio_ue4);

// Sets default values
AYAsioSampleActor::AYAsioSampleActor()
{
    this->service = nullptr; // the yasio io_service

    yasio_unreal_init();

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    VisualMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

    if (CubeVisualAsset.Succeeded())
    {
        VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
        VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    }
}

AYAsioSampleActor::~AYAsioSampleActor()
{
    if (service) {
        service->stop();
        delete service;
    }

    yasio_unreal_cleanup();
}

void AYAsioSampleActor::StartYAsioService()
{
    if (service) return;

    yasio::inet::io_hostent endpoints[] = { {"github.com", 443} };
    service = new io_service(endpoints, YASIO_ARRAYSIZE(endpoints));
    // service->set_option(YOPT_S_PRINT_FN2, &log_cb);
    service->start([=](event_ptr&& event) {
        switch (event->kind())
        {
        case YEK_PACKET: {
            auto packet = std::move(event->packet());
            //total_bytes_transferred += static_cast<int>(packet.size());
            //fwrite(packet.data(), packet.size(), 1, stdout);
            //fflush(stdout);
            packet.push_back('\0');
            FString text(UTF8_TO_TCHAR(packet.data()));
            const TCHAR* tstr = *text;
            UE_LOG(yasio_ue4, Log, TEXT("%s"), tstr);
            break;
        }
        case YEK_CONNECT_RESPONSE:
            if (event->status() == 0)
            {
                auto transport = event->transport();
                if (event->cindex() == 0)
                {
                    obstream obs;
                    obs.write_bytes("GET / HTTP/1.1\r\n");

                    obs.write_bytes("Host: github.com\r\n");

                    obs.write_bytes("User-Agent: Mozilla/5.0 (Windows NT 10.0; "
                        "WOW64) AppleWebKit/537.36 (KHTML, like Gecko) "
                        "Chrome/87.0.4280.88 Safari/537.36\r\n");
                    obs.write_bytes("Accept: */*;q=0.8\r\n");
                    obs.write_bytes("Connection: Close\r\n\r\n");

                    service->write(transport, std::move(obs.buffer()));
                }
            }
            break;
        case YEK_CONNECTION_LOST:
            // printf("The connection is lost, %d bytes transferred\n", total_bytes_transferred);
            break;
        }
        });
}

// Called when the game starts or when spawned
void AYAsioSampleActor::BeginPlay()
{
    Super::BeginPlay();

    StartYAsioService();
    service->open(0, YCK_SSL_CLIENT); // open https client
}

// Called every frame
void AYAsioSampleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    service->dispatch(128);

    FVector NewLocation = GetActorLocation();
    FRotator NewRotation = GetActorRotation();
    float RunningTime = GetGameTimeSinceCreation();
    float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
    NewLocation.Z += DeltaHeight * 20.0f;       //Scale our height by a factor of 20
    float DeltaRotation = DeltaTime * 20.0f;    //Rotate by 20 degrees per second
    NewRotation.Yaw += DeltaRotation;
    SetActorLocationAndRotation(NewLocation, NewRotation);
}

