-- free_camera.lua 

function computeDirectionFromKeys(keyboard, camera)
    local viewVec = camera:viewVector():normalized()
    local rightVec = camera:rightVector()

    local ret = vec3()

    if keyboard:keyIsPressed(Key.Z) then
        ret = viewVec
    end
    if keyboard:keyIsPressed(Key.S) then
        ret = vec3_mul(viewVec, -1)
    end
    if keyboard:keyIsPressed(Key.Q) then
        ret = vec3_mul(rightVec, -1)
    end
    if keyboard:keyIsPressed(Key.D) then
        ret = rightVec
    end

    return ret
end

function update(entity, dt)
    camera = getComponentCamera(entity)
    assert (camera)
    
    keyboard = getComponentKeyboard(entity)
    assert (keyboard)
    
    local v = vec3_add(camera.eyePos, camera.targetPos)

    local oldEye = camera.eyePos
    local oldTarget = camera.targetPos

    local speed = 100.
    if keyboard:keyIsPressed(Key.Shift) then
        speed = speed * 4.
    end
    
    local moveAmount = dt / 1000. * speed

    local moveDirection = computeDirectionFromKeys(keyboard, camera)

    local newEye = vec3_add(vec3_mul(moveDirection, moveAmount), oldEye)
    local newTarget = vec3_add(vec3_mul(moveDirection, moveAmount), oldTarget)

    camera.eyePos = newEye
    camera.targetPos = newTarget
end

