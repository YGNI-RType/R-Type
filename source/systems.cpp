/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** shared.cpp
*/

#include "GEngine/libdev/Systems.hpp"
#include "GEngine/GEngine.hpp"

#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "GEngine/libdev/systems/driver/output/Animate.hpp"
#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/systems/driver/output/FontManager.hpp"
#include "GEngine/libdev/systems/driver/output/RenderWindow.hpp"
#include "GEngine/libdev/systems/driver/output/SoundManager.hpp"
#include "GEngine/libdev/systems/driver/output/TextureManager.hpp"

#include "Constants.hpp"
#include "systems/BackgroundMotion.hpp"
#include "systems/CaterpillarsBound.hpp"
#include "systems/CaterpillarsWave.hpp"
#include "systems/ClearEntities.hpp"
#include "systems/DestroyOnCollision.hpp"
#include "systems/EnemyShoot.hpp"
#include "systems/InputsToGameEvents.hpp"
#include "systems/Invincibility.hpp"
#include "systems/PlanesAutoMotion.hpp"
#include "systems/PlanesWave.hpp"
#include "systems/PlayerAnimation.hpp"
#include "systems/PlayerMotion.hpp"
#include "systems/PlayerShoot.hpp"
#include "systems/Start.hpp"
#include "systems/UpdateScoreText.hpp"

#include "GEngine/interface/network/systems/ClientEventPublisher.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"

#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/systems/RemoteLocal.hpp"
#include "events/BecomeInvincible.hpp"

void GEngineDeclareSystems(Registry *r) {
    r->registerSystem<gengine::system::driver::output::RenderWindow>(WINDOW_WIDTH, WINDOW_TOTAL_HEIGHT, "R-Type");
    r->registerSystem<gengine::system::driver::output::Draw2D>(BLACK);
    r->registerSystem<gengine::system::driver::output::DrawSprite>();
    r->registerSystem<gengine::system::driver::output::DrawText>();
    r->registerSystem<gengine::system::driver::output::DrawRectangle>();

    r->registerSystem<gengine::system::driver::output::TextureManager>("../assets/textures");
    r->registerSystem<gengine::system::driver::output::FontManager>("../assets/fonts");
    r->registerSystem<gengine::system::driver::input::KeyboardCatcher>();
    r->registerSystem<gengine::system::driver::output::SoundManager>("../assets/sounds");
    r->registerSystem<geg::system::io::AnimationManager>("../assets/animations");
    r->registerSystem<gengine::system::driver::output::Animate>();

    r->registerSystem<gengine::system::Motion2D>();
    r->registerSystem<gengine::system::Collision2D>();
    r->registerSystem<gengine::system::AutoKiller>();

    r->registerSystem<rtype::system::InputsToGameEvents>();
    r->registerSystem<rtype::system::Start>();
    r->registerSystem<rtype::system::CaterpillarsBound>();
    r->registerSystem<rtype::system::CaterpillarsWave>();
    r->registerSystem<rtype::system::PlanesAutoMotion>();
    r->registerSystem<rtype::system::PlanesWave>();
    r->registerSystem<rtype::system::PlayerMotion>();
    r->registerSystem<rtype::system::PlayerAnimation>();
    r->registerSystem<rtype::system::PlayerShoot>();
    r->registerSystem<rtype::system::BackgroundMotion>();
    r->registerSystem<rtype::system::ClearEntities>();
    r->registerSystem<rtype::system::DestroyOnCollision>();
    r->registerSystem<rtype::system::UpdateScoreText>();
    r->registerSystem<rtype::system::EnemyShoot>();
    r->registerSystem<rtype::system::Invincibility>();

    r->registerSystem<gengine::interface::system::HandleRemoteLocal>();
    r->registerSystem<gengine::interface::system::HandleLocal>();

    r->registerSystem<gengine::interface::network::system::ClientEventPublisher<
        rtype::event::Movement, rtype::event::Shoot, gengine::interface::event::GetRemoteLocalWhoIAm,
        rtype::event::BecomeInvincible>>();
    r->registerSystem<gengine::interface::network::system::ServerEventReceiver<
        rtype::event::Movement, rtype::event::Shoot, gengine::interface::event::GetRemoteLocalWhoIAm>>();

    // TODO auto register ↓
}