import java.io.bufferedinputstream;
import java.io.fileinputstream;
import java.io.inputstream;
import java.io.ioexception;
import javazoom.jl.decoder.javalayerexception;
import javazoom.jl.player.audiodevice;
import javazoom.jl.player.factoryregistry;
import javazoom.jl.player.advanced.advancedplayer;

/**
 * provide basic playing of mp3 files via the javazoom library.
 * see http://www.javazoom.net/
 * 
 * @author david j. barnes and michael kölling.
 * @version 2016.02.29
 */
public class musicplayer
{
    // the current player. it might be null.
    private advancedplayer player;
    
    /**
     * constructor for objects of class musicfileplayer
     */
    public musicplayer()
    {
        player = null;
    }
    
    /**
     * play a part of the given file.
     * the method returns once it has finished playing.
     * @param filename the file to be played.
     */
    public void playsample(string filename)
    {
        try {
            setupplayer(filename);
            player.play(500);
        }
        catch(javalayerexception e) {
            reportproblem(filename);
        }
        finally {
            killplayer();
        }
    }
    
    /**
     * start playing the given audio file.
     * the method returns once the playing has been started.
     * @param filename the file to be played.
     */
    public void startplaying(final string filename)
    {
        try {
            setupplayer(filename);
            thread playerthread = new thread() {
                public void run()
                {
                    try {
                        player.play(5000);
                    }
                    catch(javalayerexception e) {
                        reportproblem(filename);
                    }
                    finally {
                        killplayer();
                    }
                }
            };
            playerthread.start();
        }
        catch (exception ex) {
            reportproblem(filename);
        }
    }
    
    public void stop()
    {
        killplayer();
    }
    
    /**
     * set up the player ready to play the given file.
     * @param filename the name of the file to play.
     */
    private void setupplayer(string filename)
    {
        try {
            inputstream is = getinputstream(filename);
            player = new advancedplayer(is, createaudiodevice());
        }
        catch (ioexception e) {
            reportproblem(filename);
            killplayer();
        }
        catch(javalayerexception e) {
            reportproblem(filename);
            killplayer();
        }
    }

    /**
     * return an inputstream for the given file.
     * @param filename the file to be opened.
     * @throws ioexception if the file cannot be opened.
     * @return an input stream for the file.
     */
    private inputstream getinputstream(string filename)
        throws ioexception
    {
        return new bufferedinputstream(
                    new fileinputstream(filename));
    }

    /**
     * create an audio device.
     * @throws javalayerexception if the device cannot be created.
     * @return an audio device.
     */
    private audiodevice createaudiodevice()
        throws javalayerexception
    {
        return factoryregistry.systemregistry().createaudiodevice();
    }

    /**
     * terminate the player, if there is one.
     */
    private void killplayer()
    {
        synchronized(this) {
            if(player != null) {
                player.stop();
                player = null;
            }
        }
    }
    
    /**
     * report a problem playing the given file.
     * @param filename the file being played.
     */
    private void reportproblem(string filename)
    {
        system.out.println("there was a problem playing: " + filename);
    }

}
