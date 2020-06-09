// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Reflection;

public static class Program
{
    public static int Main(string[] args)
    {
	var assemblyName = new AssemblyName ("runningmono"); // not "Foo.dll", but the name of the assembly
	var assembly = Assembly.Load (assemblyName);
	var mainMethod = assembly.EntryPoint;
	mainMethod.Invoke (null, args);


        Console.WriteLine("Hello, Android!"); // logcat
        return 42;
    }
}
