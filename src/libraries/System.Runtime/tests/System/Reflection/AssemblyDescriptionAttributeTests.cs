// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using Xunit;

namespace System.Reflection.Tests
{
    public class AssemblyDescriptionAttributeTests
    {
        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("description")]
        public void Ctor_String(string description)
        {
            var attribute = new AssemblyDescriptionAttribute(description);
            Assert.Equal(description, attribute.Description);
        }
    }
}
