//////////////////////////////////////////////////////////////////////////////
//
// License Agreement:
//
// The following are Copyright � 2007, mC2 Team
//
// Sources and Binaries of: mC2
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright 
//      notice, this list of conditions and the following disclaimer in the 
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may 
//      be used to endorse or promote products derived from this software 
//      without specific prior written permission. 
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE. 
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.hpp"
#include <win32cpp/ApplicationThread.hpp>
#include <cube/settings/SyncPathModel.hpp>
#include <cube/SettingsController.hpp>
#include <core/Indexer.h>
#include <core/LibraryFactory.h>

//////////////////////////////////////////////////////////////////////////////

using namespace musik::cube::settings;

//////////////////////////////////////////////////////////////////////////////

SyncPathModel::SyncPathModel(SyncPathController *controller)
: controller(controller)
{
    musik::core::Indexer *indexer   = this->controller->settingsController->library->Indexer();
    if(indexer){
        indexer->PathsUpdated.connect(this,&SyncPathModel::OnPathsUpdated);
    }
    this->UpdateSyncPaths();

}


uistring SyncPathModel::CellValueToString(int rowIndex, ListView::ColumnRef column){
    if(rowIndex<this->paths.size() && rowIndex>=0){
        return this->paths[rowIndex];
    }

    return uistring();
}

void SyncPathModel::UpdateSyncPaths(){
    musik::core::Indexer *indexer   = this->controller->settingsController->library->Indexer();
    if(indexer){
        this->paths    = indexer->GetPaths();
    }

    this->SetRowCount(0);
    this->SetRowCount((int)this->paths.size());

}

void SyncPathModel::OnPathsUpdated(){
    if(!win32cpp::ApplicationThread::InMainThread()){
        win32cpp::ApplicationThread::Call0(this,&SyncPathModel::OnPathsUpdated);
        return;
    }

    this->UpdateSyncPaths();

}

