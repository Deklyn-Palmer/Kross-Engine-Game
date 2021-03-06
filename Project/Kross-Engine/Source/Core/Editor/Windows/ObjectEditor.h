/*
 *  Author: Chris Deitch.
 *  Editors:
 *      - Chris Deitch.
 */

#pragma once
#include "../../Core.h"
#include "../EditorWindow.h"


namespace Kross
{
    class KROSS_API ObjectEditor : public EditorWindow 
    {
    public:

        ObjectEditor() :
            p_SelectedObject    (nullptr),
            p_PreviewPane       (nullptr)
        {};

        ~ObjectEditor() {};

    private:
        Object* p_SelectedObject;
        AssetPreview* p_PreviewPane;

    protected:
        friend class SceneHierarchy;
        friend class Editor;

        void Attach() override;
        
        void SetFlags() override;

        void OnStart() override;

    };
}