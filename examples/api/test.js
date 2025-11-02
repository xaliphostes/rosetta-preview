const myapi = require('./generated/build/Release/myapi')

const mesh = new myapi.Mesh()
console.log(mesh.getVertexCount())
mesh.addVertex(new myapi.Point(0,0,0))
