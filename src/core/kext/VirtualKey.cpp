#include "base.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "VirtualKey.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  bool
  Handle_VK_LOCK::handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    Flags flags(0);
    bool force_on = false;
    bool force_off = false;

    if (params.key == KeyCode::VK_LOCK_FN) {
      flags.add(ModifierFlag::FN);
    } else if (params.key == KeyCode::VK_LOCK_FN_FORCE_OFF) {
      flags.add(ModifierFlag::FN);
      force_off = true;
    } else if (params.key == KeyCode::VK_LOCK_COMMAND_R) {
      flags.add(ModifierFlag::COMMAND_R);

    } else if (params.key == KeyCode::VK_LOCK_ALL_FORCE_OFF) {
      FlagStatus::lock_clear();
      return true;
    } else {
      return false;
    }

    if (params.eventType == EventType::DOWN && params.repeat == false) {
      if (force_off) {
        FlagStatus::lock_decrease(flags);
      } else if (force_on) {
        FlagStatus::lock_increase(flags);
      } else {
        FlagStatus::lock_toggle(flags);
      }
    }

    return true;
  }

  // ----------------------------------------------------------------------
  bool
  Handle_VK_CHANGE_INPUTMODE::handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    KeyRemap4MacBook_bridge::ChangeInputMode::Request request;
    if (params.key == KeyCode::VK_CHANGE_INPUTMODE_ENGLISH) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_ENGLISH;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_FRENCH) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_FRENCH;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_GERMAN) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_GERMAN;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_JAPANESE) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_JAPANESE;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_SWEDISH) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_SWEDISH;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_CANADIAN) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_CANADIAN;
    } else {
      return false;
    }

    if (params.eventType == EventType::DOWN && params.repeat == false) {
      KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_CHANGE_INPUTMODE, &request, sizeof(request), NULL, 0);
    }
    return true;
  }

  // ----------------------------------------------------------------------
  bool
  Handle_VK_CONFIG::handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    int* configitem = NULL;

    enum Type {
      TYPE_TOGGLE,
      TYPE_FORCE_ON,
      TYPE_FORCE_OFF,
    } type = TYPE_TOGGLE;

#include "config/output/include.remapcode_vk_config.cpp"

    if (configitem == NULL) {
      return false;
    }

    // ------------------------------------------------------------
    if (params.eventType == EventType::DOWN && params.repeat == false) {
      switch (type) {
        case TYPE_TOGGLE:
          *configitem = ! *configitem;
          break;
        case TYPE_FORCE_ON:
          *configitem = 1;
          break;
        case TYPE_FORCE_OFF:
          *configitem = 0;
          break;
      }
      refresh_remapfunc();
    }
    return true;
  }

  // ----------------------------------------------------------------------
  bool
  Handle_VK_JIS_TOGGLE_EISUU_KANA::handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_TOGGLE_EISUU_KANA) return false;

    if (params.eventType == EventType::DOWN) {
      if (workspacedata.inputmode == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE) {
        newkeycode_ = KeyCode::JIS_EISUU;
      } else {
        newkeycode_ = KeyCode::JIS_KANA;
      }
    }

    params.key = newkeycode_;
    RemapUtil::fireKey(params, workspacedata);
    return true;
  }

  KeyCode Handle_VK_JIS_TOGGLE_EISUU_KANA::newkeycode_;

  // ----------------------------------------------------------------------
  bool
  handle_VK_JIS_EISUU_x2(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_EISUU_x2) return false;

    if (params.eventType == EventType::DOWN) {
      for (int i = 0; i < 2; ++i) {
        RemapUtil::fireKey_downup(params.flags, KeyCode::JIS_EISUU, params.keyboardType, workspacedata);
      }
    }
    return true;
  }

  bool
  handle_VK_JIS_KANA_x2(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_KANA_x2) return false;

    if (params.eventType == EventType::DOWN) {
      for (int i = 0; i < 2; ++i) {
        RemapUtil::fireKey_downup(params.flags, KeyCode::JIS_KANA, params.keyboardType, workspacedata);
      }
    }
    return true;
  }

  bool
  handle_VK_JIS_BACKSLASH(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_BACKSLASH) return false;

    params.key = KeyCode::BACKSLASH;
    params.keyboardType = KeyboardType::MACBOOK;
    RemapUtil::fireKey(params, workspacedata);
    return true;
  }

  // ----------------------------------------------------------------------
  bool
  Handle_VK_JIS_TEMPORARY::handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata,
                                  KeyCode key,
                                  const KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail& inputmodedetail)
  {
    if (params.key != key) return false;

    if (params.eventType == EventType::DOWN) {
      if (savedinputmodedetail_ == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE) {
        savedinputmodedetail_ = workspacedata.inputmodedetail;
        currentinputmodedetail_ = workspacedata.inputmodedetail;
      }
      firekeytoinputdetail(params, workspacedata, inputmodedetail);
    }

    return true;
  }

  bool
  Handle_VK_JIS_TEMPORARY::handle_RESTORE(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_TEMPORARY_RESTORE) return false;

    if (params.eventType == EventType::DOWN) {
      if (savedinputmodedetail_ != KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE) {
        firekeytoinputdetail(params, workspacedata, savedinputmodedetail_);
        savedinputmodedetail_ = KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE;
        currentinputmodedetail_ = KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE;
      }
    }

    return true;
  }

  void
  Handle_VK_JIS_TEMPORARY::firekeytoinputdetail(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata,
                                                KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail inputmodedetail)
  {
    inputmodedetail = normalize(inputmodedetail);
    currentinputmodedetail_ = normalize(currentinputmodedetail_);

    // ------------------------------------------------------------
    if (inputmodedetail == currentinputmodedetail_) return;
    if (inputmodedetail == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE) return;

    currentinputmodedetail_ = inputmodedetail;

    // ------------------------------------------------------------
    if (inputmodedetail == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN) {
      RemapUtil::fireKey_downup(Flags(0), KeyCode::JIS_EISUU, params.keyboardType, workspacedata);
      return;
    }

    if (inputmodedetail == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HIRAGANA) {
      RemapUtil::fireKey_downup(Flags(0), KeyCode::JIS_KANA, params.keyboardType, workspacedata);
      return;
    }

    if (inputmodedetail == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_KATAKANA) {
      RemapUtil::fireKey_downup(ModifierFlag::SHIFT_L, KeyCode::JIS_KANA, params.keyboardType, workspacedata);
      return;
    }
  }

  KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail
  Handle_VK_JIS_TEMPORARY::normalize(const KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail& imd)
  {
    if (imd == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE) {
      return KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HIRAGANA;
    }
    return imd;
  }

  KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail Handle_VK_JIS_TEMPORARY::savedinputmodedetail_ = KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE;
  KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail Handle_VK_JIS_TEMPORARY::currentinputmodedetail_ = KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE;
}
