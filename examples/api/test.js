const myapi = require('./generated/build/Release/myapi')

// Create a point
const p = new myapi.Point(1, 2, 3);
console.log("Point created:", p.x(), p.y(), p.z());

// Create a mesh
const mesh = new myapi.Mesh();
console.log("Initial vertex count:", mesh.getVertexCount());

// Add the point to the mesh
mesh.addVertex(p);
console.log("After adding vertex:", mesh.getVertexCount());

mesh.vertices.forEach( v => {
    console.log(v.magnitude())
})