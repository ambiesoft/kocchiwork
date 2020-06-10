using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace kocchichoose
{
    class LVInfo
    {
        public enum LVState
        {
            LVState_Normal,
            LVState_NotFound,
            LVState_Exception,
        } ;
        LVState lvState_;
        public LVInfo(LVState state)
        {
            lvState_ = state;
        }
        
        public bool IsError
        {
            get { return lvState_ != LVState.LVState_Normal; }
        }
    }
}
