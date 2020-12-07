// Fill out your copyright notice in the Description page of Project Settings.


#include "YAsioActor.h"
#include "yasio/platform/yasio_ue4.hpp"

DECLARE_LOG_CATEGORY_EXTERN(yasio_ue4, Log, All);
DEFINE_LOG_CATEGORY(yasio_ue4);

// Sets default values
AYAsioActor::AYAsioActor()
{
    this->service = nullptr; // the yasio io_service

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

AYAsioActor::~AYAsioActor()
{
    if (service) {
        service->stop();
        delete service;
    }
}

void AYAsioActor::InitYAsio()
{
    if (service) return;

    print_fn2_t log_cb = [](int level, const char* msg) {
        FString text(msg);
        const TCHAR* tstr = *text;
        UE_LOG(yasio_ue4, Log, L"%s", tstr);
    };
    io_service::init_globals(log_cb);
	
    yasio::inet::io_hostent endpoints[] = { {"tool.chinaz.com", 80} };
    service = new io_service(endpoints, YASIO_ARRAYSIZE(endpoints));
    service->set_option(YOPT_S_PRINT_FN2, &log_cb);
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
            UE_LOG(yasio_ue4, Log, L"%s", tstr);
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

                    obs.write_bytes("Host: tool.chinaz.com\r\n");

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
void AYAsioActor::BeginPlay()
{
    Super::BeginPlay();

    InitYAsio();
    service->open(0); // open http client
}

// Called every frame
void AYAsioActor::Tick(float DeltaTime)
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

