#pragma once

#include <vector>
#include <string>

namespace ModelingData
{
    struct ZNode
    {
        ZNode* parent;
        std::vector<ZNode*> children;
        std::vector<ZMesh*> meshes;
    };

    struct ZVertex
    {
        float x, y, z; // ������ ��ġ ��ǥ
        float nx, ny, nz; // ������ ���� ���� ��ǥ
        float u, v; // �ؽ�ó ��ǥ
        float r, g, b, a; // ��
    };

    struct ZMesh
    {
        bool hasPosition;

        std::vector<ZVertex> vertices; // ���� ������
        std::vector<unsigned int> indices; // ���� �ε��� ������
    };

    struct ZMaterial
    {
        // ���׸��� ����: �ں��Ʈ, ��ǻ��, ����ŧ�� ����, �ؽ�ó ��� ���� ������ �� �ֽ��ϴ�.
        float ambient[4]; // �ں��Ʈ ���� (R, G, B, A)
        float diffuse[4]; // ��ǻ�� ���� (R, G, B, A)
        float specular[4]; // ����ŧ�� ���� (R, G, B, A)
        float shininess; // ����ŧ�� ����
        std::wstring texturePath; // ���׸��� ���� �ؽ�ó ���� ���
    };

    struct ZModel
    {
        ZNode* rootNode;
        std::vector<ZMesh> meshes; // ���� �޽� ������
        std::vector<ZMaterial> materials;
    };


}