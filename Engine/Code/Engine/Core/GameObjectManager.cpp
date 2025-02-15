#include "Engine/Core/GameObjectManager.hpp"
#include "Engine/Core/GameObject.hpp"
#include "Engine/Core/ContainerUtils.hpp"

GameObjectManager* GameObjectManager::s_default = nullptr;

GameObjectManager* GameObjectManager::GetDefault()
{
    if( !s_default )
        s_default = new GameObjectManager();

    return s_default;
}

void GameObjectManager::Update()
{
    for (int i = 0; i < m_allGameObjectsFlat.size() ; ++i)
    {
        m_allGameObjectsFlat[i]->Update();
    }
}

void GameObjectManager::DeleteDeadGameObjects()
{
    int count = (int)m_allGameObjectsFlat.size();
    // loop removes elements from vectors!
    for (int i = count - 1; i >= 0 ; --i)
    {
        GameObject* go = m_allGameObjectsFlat[i];
        if( go->ShouldDie() )
        {
            delete go;
        }
    }
}

std::vector<GameObject*>& GameObjectManager::GetObjectsOfType( std::string type )
{
    return m_allGameObjects[type];
}

std::vector<GameObject*>& GameObjectManager::GetObejctsFlat()
{
    return m_allGameObjectsFlat;
}

void GameObjectManager::AddGameObject( GameObject* go )
{
    std::string goType = go->GetType();
    std::vector<GameObject*>& gos = GetObjectsOfType( goType );

    gos.push_back( go );
    m_allGameObjectsFlat.push_back( go );
}

void GameObjectManager::RemoveGameObject( GameObject* go )
{
    std::string goType = go->GetType();
    std::vector<GameObject*>& gos = GetObjectsOfType( goType );
    ContainerUtils::EraseOneValue( gos, go );
    ContainerUtils::EraseOneValue( m_allGameObjectsFlat, go );
}

