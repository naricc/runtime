// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
using System;
using System.Runtime.CompilerServices;

namespace TestRunningMono
{
    class Program
    {
        public static int Main(string[] args)
        {
             const int Pass = 100, Fail = 1;
             bool isMono = typeof(object).Assembly.GetType("Mono.RuntimeStructs") != null;

             bool isInterpreter = RuntimeFeature.IsDynamicCodeSupported == true && RuntimeFeature.IsDynamicCodeCompiled == false;

             if(isMono && isInterpreter)
             {
                 return Pass;
             }
             else
             {
                 return Fail;
             }
        }
   }
}

