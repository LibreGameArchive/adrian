﻿/*
---------------------------------------------------------------------------
Open Asset Import Library (ASSIMP)
---------------------------------------------------------------------------

Copyright (c) 2006-2008, ASSIMP Development Team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ASSIMP Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

using System;
using System.Collections.Generic;
using System.Text;

namespace Assimp.NET
{
    public class Importer
    {
        public Importer()
        {
            throw new System.NotImplementedException();
        }
        
        public Importer(int version)
        {
            throw new System.NotImplementedException();
        }

        public static int PROPERTY_WAS_NOT_EXISTING;

        public bool addPostProcessStep(PostProcessStep ppStep)
        {
            throw new System.NotImplementedException();
        }

        public long getContext()
        {
            throw new System.NotImplementedException();
        }

        public IOStream getIOSystem()
        {
            throw new System.NotImplementedException();
        }

        public int getPropertyInt(String property)
        {
            throw new System.NotImplementedException();
        }

        public int getPropertyInt(String property, int error_return)
        {
            throw new System.NotImplementedException();
        }

        public bool isDefaultIOSystem()
        {
            throw new System.NotImplementedException();
        }

        public bool isPostProcessStepActive(PostProcessStep ppStep)
        {
            throw new System.NotImplementedException();
        }

        public Scene readFile(String path)
        {
            throw new System.NotImplementedException();
        }

        public bool removePostProcessStep(PostProcessStep ppStep)
        {
            throw new System.NotImplementedException();
        }

        public int setPropertyInt(String property, int val)
        {
            throw new System.NotImplementedException();
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            return (Importer)obj == this;
        }

        public override string ToString()
        {
            throw new System.NotImplementedException();
        }
    }
}
