const addon = require('./build/Release/complex.node');

const surface = new addon.Surface(
    [0.1, 0.1, 0.1, 1.1, 0.1, 0.1, 0.1, 1.1, 0.1],
    [0, 1, 2]
)

console.log("points")
surface.points.forEach(p => console.log(p.x, p.y, p.z))

console.log("triangles")
surface.triangles.forEach(t => console.log(t.a, t.b, t.c))

const model = new addon.Model()
model.addSurface(surface)

console.log("surface")
model.surfaces.forEach(s => {
    s.points.forEach(p => {
        console.log(p.x, p.y, p.z)
    })
    s.triangles.forEach(t => {
        console.log(t.a, t.b, t.c)
    })
})
