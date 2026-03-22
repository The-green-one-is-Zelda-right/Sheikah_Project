# Sheikah_Project

KOCCA 4th Final Project의 핵심 C++ 개발본입니다.
이 프로젝트는 단순 게임 클라이언트가 아니라, 커스텀 엔진 `PurahEngine`, 게임 로직 `ZeldaClient`, 렌더링 모듈 `ZeldaGraphics`, 물리 모듈 `ZonaiPhysicsX`를 함께 구성한 통합 프로젝트입니다.

## 프로젝트 성격

- 커스텀 C++ 엔진 기반 프로젝트
- 퍼즐 중심 게임 플레이 시스템 구현 프로젝트
- 렌더링, 물리, 씬 로딩, 데이터 직렬화까지 직접 다룬 팀 프로젝트

## 현재 기준 메인 프로젝트

이 저장소 전체 기준에서 실제 메인 개발본은 이 `Sheikah_Project` 폴더입니다.
루트의 `5_Project/PhyzzleUnity`는 Unity 기반 프로토타입 또는 병행 개발 흔적으로 보는 것이 적절합니다.

## 솔루션 구조

메인 솔루션 파일:

- `PurahEngine.sln`

주요 프로젝트:

- `PurahEngine`
  - 엔진 본체 DLL
  - 게임 루프, 씬 관리, 입력, 사운드, 직렬화, 그래픽/물리 연동 담당
- `ZeldaClient`
  - 실제 게임 플레이 로직과 콘텐츠 코드
  - 플레이어, 퍼즐 장치, 씬 파일, 게임 규칙 포함
- `ZeldaGraphics`
  - DirectX 11 기반 렌더링 모듈
  - 셰이더와 렌더링 리소스 관리 포함
- `ZonaiPhysicsBase`
  - 물리 공통 인터페이스와 기본 타입
- `ZonaiPhysicsX`
  - PhysX 기반 물리 구현체
- `ZeldaFBXLoader`
  - FBX 로더
- `GraphicsTest`
  - 그래픽 테스트용 프로젝트

## 실행 구조

실행 시작점은 `ZeldaClient/main.cpp` 입니다.

실행 흐름은 다음과 같습니다.

1. `ZeldaClient`가 `PurahEngine.dll`을 동적으로 로드합니다.
2. `ComponentFactory`에 게임 컴포넌트를 등록합니다.
3. 엔진 `Initialize()`를 호출합니다.
4. 엔진 내부에서 다음 순서로 초기화가 진행됩니다.
   - EngineSettings 로드
   - 시간 관리자 초기화
   - GraphicsManager 초기화
   - Input/GamePad/UnifiedInput 초기화
   - SceneManager 초기화
   - PhysicsSystem 초기화
   - SoundManager 초기화
   - 첫 씬 로드
5. 메인 루프에서 물리, 입력, 씬 업데이트, 애니메이션, 사운드, 렌더링이 순차적으로 수행됩니다.
6. 종료 시 `Finalize()`로 엔진 리소스를 정리합니다.

## 데이터 중심 구성

핵심 설정 파일:

- `ZeldaClient/EngineSettings.json`

이 파일에는 다음 데이터가 들어 있습니다.

- 태그
- 레이어
- 충돌 레이어 매트릭스
- 물리 재질
- 프리로드 모델 목록
- 씬 목록
- 그림자 설정
- 중력 설정

씬 데이터는 `.pzscene` 및 `.json` 파일 형태로 관리됩니다.

관련 엔진 파일:

- `PurahEngine/SceneManager.cpp`
- `PurahEngine/FileManager.cpp`
- `PurahEngine/DataManager.cpp`

## 눈여겨볼 점

### 1. 커스텀 엔진 기반

이 프로젝트의 가장 큰 강점은 기존 상용 엔진 위에서 게임만 만든 형태가 아니라,
엔진 계층과 게임 계층을 직접 분리해서 구축했다는 점입니다.

### 2. 퍼즐 시스템 중심 구조

`ZeldaClient`에는 다음과 같은 퍼즐 관련 시스템이 존재합니다.

- `AttachSystem`
- `RewindSystem`
- `PowerController`
- `TriggerDevice`
- `ButtonDevice`
- `ElectricWire`
- `Door`
- `RespawnSystem`

즉, 단순 액션 구현보다 상호작용 규칙과 퍼즐 기믹 설계 비중이 큰 프로젝트입니다.

### 3. 모듈 분리

엔진, 게임 로직, 렌더링, 물리, FBX 로더가 프로젝트 단위로 분리되어 있어 구조를 이해하기 좋습니다.

### 4. 데이터 드리븐 설계

씬, 태그, 레이어, 물리 재질, 프리로드 모델 목록을 코드 밖 데이터로 관리하고 있습니다.
이 점은 유지보수와 콘텐츠 확장 측면에서 중요한 특징입니다.

## 기술 스택

- 언어: C++, HLSL, JSON
- 빌드 환경: Visual Studio solution / vcxproj
- 렌더링: DirectX 11 기반 커스텀 렌더러
- 물리: PhysX 래핑 모듈
- 수학: Eigen
- 모델 로딩: Assimp 기반 FBX 로더
- 오디오: FMOD 관련 폴더 존재

## 빌드 시 참고 사항

확인된 사실 기준으로 보면 Visual Studio 2022 환경에서 작업된 솔루션입니다.

솔루션에는 여러 설정이 존재하지만, 현재 구조상 가장 먼저 확인해볼 후보는 다음과 같습니다.

- `Debug | x64`
- `Release | x64`

주의할 점:

- 솔루션 설정 종류가 많아 실제 사용 중인 설정을 팀 기준으로 다시 정리할 필요가 있습니다.
- 외부 라이브러리와 DLL 의존성이 많으므로, 실행 전에 출력 폴더와 DLL 배치 상태를 확인해야 합니다.
- `ZeldaClient`는 `PurahEngine.dll`을 동적으로 로드하므로 DLL 위치가 맞지 않으면 실행되지 않습니다.

## 처음 읽을 때 추천 순서

- `PurahEngine.sln`
- `ZeldaClient/main.cpp`
- `PurahEngine/GameLoop.cpp`
- `ZeldaClient/EngineSettings.json`
- `PurahEngine/SceneManager.cpp`
- `ZeldaClient/AttachSystem.cpp`
- `ZeldaClient/RewindSystem.cpp`
- `ZeldaClient/PowerController.cpp`
- `ZeldaGraphics/*`
- `ZonaiPhysicsX/*`

## 현재 보이는 개선 필요 사항

- 저장소에 산출물과 생성 폴더가 많이 섞여 있음
- 솔루션 설정이 많아 빌드 기준이 불명확함
- README와 온보딩 문서가 부족했음
- 일부 설정 API와 물리 시스템 코드에 안정성 문제가 있음
- 엔진 코드 안에 게임 전용 입력/디버그 규칙이 일부 섞여 있음

## 한 줄 정리

`Sheikah_Project`는 커스텀 엔진, 렌더링, 물리, 데이터 로딩, 퍼즐 시스템을 함께 만든 이 저장소의 핵심 자산입니다.
포트폴리오 관점에서는 "게임 하나"보다 "엔진과 퍼즐 시스템을 통합 설계한 프로젝트"로 설명하는 것이 더 강합니다.